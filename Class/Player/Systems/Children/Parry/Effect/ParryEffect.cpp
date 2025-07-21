#include "ParryEffect.h"
#include "../../../../../Camera/FollowCamera.h"
#include "../../../../../Player/Player.h"
#include "../../../../Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

ParryEffect::ParryEffect(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;
}

void ParryEffect::Initialize() {

}

void ParryEffect::Update() {
	//IEffect::Update();
	for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		// 生存時間が過ぎたら処理を行わない
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		// 各パーティクルの更新処理
		(*particleIterator).updateFunc(*particleIterator);

		// 生存時間
		(*particleIterator).currentTime++;

		particleIterator++;
	}
}

void ParryEffect::DebugGui() {
	// パーティクル数
	//jsonData_.count = lineParticleCount + circleParticleCount;
	//DebugGUI();
}

void ParryEffect::MakeLineParticle(ParticleData& particle, Vector3 pos) {
	// パーティクルの種類
	particle.type = (int)ParticleType::kLine;

	// ビルボード生成(非表示)
	particle.billboard.anchorPoint = { 0.5f, 0.5f };
	particle.billboard.material.texture = LWP::Resource::LoadTexture("Effect/Particle.png");
	// 平面生成
	particle.plane.LoadShortPath("BothPlane.obj");
	particle.plane.isActive = false;

	// 色
	float kelvin = LWP::Utility::Random::GenerateFloat(lineParticleKelvin.min, lineParticleKelvin.max);
	particle.billboard.material.color = LWP::Utility::Color::KelvinToRGB(kelvin);

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(lineParticleData_.velocity.min, lineParticleData_.velocity.max);
	particle.vel = vel;
	particle.vel.y += lineParticleData_.firstVel.y;

	// 座標
	particle.billboard.worldTF.translation = pos;
	// 大きさ
	Vector3 scale = LWP::Utility::Random::GenerateVector3(lineParticleData_.scale.min, lineParticleData_.scale.max);
	particle.billboard.worldTF.scale = scale;

	// 生存可能時間
	particle.lifeTime = lineParticleData_.maxElapseTime * 60.0f;
	particle.currentTime = 0;

	// 更新処理を設定
	particle.updateFunc = std::bind(&ParryEffect::LineParticleUpdate, this, std::placeholders::_1);
}

void ParryEffect::MakeCircleParticle(ParticleData& particle, LWP::Math::Vector3 pos) {
	// パーティクルの種類
	particle.type = (int)ParticleType::kCircle;

	// ビルボード生成
	particle.billboard.anchorPoint = { 0.5f, 0.5f };
	particle.billboard.material.enableLighting = false;
	particle.billboard.material.texture = LWP::Resource::LoadTexture("Effect/Particle.png");
	// 平面生成(非表示)
	particle.plane.LoadShortPath("BothPlane.obj");
	particle.plane.isActive = false;

	// 色
	//float kelvin = LWP::Utility::Random::GenerateFloat(lineParticleKelvin.min, lineParticleKelvin.max);
	//particle.plane.materials[""].color = LWP::Utility::Color::KelvinToRGB(kelvin);
	particle.billboard.material.color.A = 122;
	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(circleParticleData_.velocity.min, circleParticleData_.velocity.max);
	particle.vel = vel;
	particle.vel.y += circleParticleData_.firstVel.y;

	// 座標
	particle.billboard.worldTF.translation = pos;
	// 大きさ
	Vector3 scale = LWP::Utility::Random::GenerateVector3(circleParticleData_.scale.min, circleParticleData_.scale.max);
	particle.billboard.worldTF.scale = {
		scale.x,
		scale.x,
		scale.x
	};

	// 生存可能時間
	particle.lifeTime = circleParticleData_.maxElapseTime * 60.0f;
	particle.currentTime = 0;

	// 更新処理を設定
	particle.updateFunc = std::bind(&ParryEffect::CircleParticleUpdate, this, std::placeholders::_1);
}

void ParryEffect::MakeLargeFlashParticle(ParticleData& particle, LWP::Math::Vector3 pos) {
	// パーティクルの種類
	particle.type = (int)ParticleType::kLargeFlash;

	// ビルボード生成(非表示)
	particle.billboard.anchorPoint = { 0.5f, 0.5f };
	particle.billboard.material.texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	particle.billboard.worldTF.rotation *= LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, 0.2f);
	// 平面生成
	particle.plane.LoadShortPath("BothPlane.obj");
	particle.plane.isActive = false;

	// アヤメ色に変更
	particle.billboard.material.color = { 199, 126, 181, 122 };

	// 座標
	particle.billboard.worldTF.translation = pos;
	// 大きさ
	particle.billboard.worldTF.scale = { 0,0,0 };

	// 角度
	Vector3 dist = (followCamera_->GetCamera()->worldTF.GetWorldPosition() - particle.billboard.worldTF.GetWorldPosition()).Normalize();
	Vector3 radian = {
		0.0f,
		std::atan2(dist.x, dist.z) + LWP::Utility::Random::GenerateFloat(LWP::Utility::DegreeToRadian(-30.0f),LWP::Utility::DegreeToRadian(30.0f)),
		LWP::Utility::Random::GenerateFloat(LWP::Utility::DegreeToRadian(60.0f), LWP::Utility::DegreeToRadian(120.0f))
	};
	particle.euler = radian;
	particle.billboard.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, radian.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, radian.x);
	particle.billboard.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, radian.y) * particle.plane.worldTF.rotation;

	// 生存可能時間
	particle.lifeTime = largeFlashData_.maxElapseTime * 60.0f;
	particle.currentTime = 0;

	// 更新処理を設定
	particle.updateFunc = std::bind(&ParryEffect::LargeFlashUpdate, this, std::placeholders::_1);
}

void ParryEffect::MakeShortFlashParticle(ParticleData& particle, LWP::Math::Vector3 pos) {
	// パーティクルの種類
	particle.type = (int)ParticleType::kShortFlash;

	// ビルボード生成(非表示)
	particle.billboard.anchorPoint = { 0.5f, 0.5f };
	particle.billboard.material.texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	// 平面生成
	particle.plane.LoadShortPath("BothPlane.obj");
	particle.plane.isActive = false;

	// 黄色に変更
	particle.billboard.material.color = { 255,212,71,255 };
	particle.plane.materials["Texturematerial"].color = { 255,212,71,122 };

	// 座標
	particle.billboard.worldTF.translation = pos;
	particle.plane.worldTF.translation = pos;
	// 大きさ
	particle.billboard.worldTF.scale = { 0,0,0 };
	particle.plane.worldTF.scale = { 0,0,0 };

	// 角度
	Vector3 dist = (followCamera_->GetCamera()->worldTF.GetWorldPosition() - particle.billboard.worldTF.GetWorldPosition()).Normalize();
	Vector3 radian = {
		0.0f,
		0.0f,
		LWP::Utility::Random::GenerateFloat(LWP::Utility::DegreeToRadian(0.0f), LWP::Utility::DegreeToRadian(180.0f))
	};
	particle.euler = radian;
	particle.billboard.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, radian.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, radian.x);
	particle.billboard.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, radian.y) * particle.plane.worldTF.rotation;

	// 生存可能時間
	particle.lifeTime = shortFlashData_.maxElapseTime * 60.0f;
	particle.currentTime = 0;

	// 更新処理を設定
	particle.updateFunc = std::bind(&ParryEffect::ShortFlashUpdate, this, std::placeholders::_1);
}

void ParryEffect::MakeRingParticle(ParticleData& particle, LWP::Math::Vector3 pos) {
	// パーティクルの種類
	particle.type = (int)ParticleType::kRing;

	// ビルボード生成
	particle.billboard.anchorPoint = { 0.5f, 0.5f };
	particle.billboard.material.texture = LWP::Resource::LoadTexture("Effect/CircleParticle.png");
	// 平面生成(非表示)
	particle.plane.LoadShortPath("BothPlane.obj");
	particle.plane.isActive = false;

	// 座標
	particle.billboard.worldTF.translation = pos;
	// 大きさ
	particle.billboard.worldTF.scale = { 0,0,0 };

	// 生存可能時間
	particle.lifeTime = ringData_.maxElapseTime * 60.0f;
	particle.currentTime = 0;

	// 更新処理を設定
	particle.updateFunc = std::bind(&ParryEffect::RingParticleUpdate, this, std::placeholders::_1);
}

std::list<ParticleData> ParryEffect::JustEmission(LWP::Math::Vector3 pos) {
	std::list<ParticleData> particles;

	// 線パーティクル
	for (int32_t count = 0; count < lineParticleData_.count; ++count) {
		particles.emplace_back();
		MakeLineParticle(particles.back(), pos);
	}
	// 円パーティクル
	for (int32_t count = 0; count < circleParticleData_.count; ++count) {
		particles.emplace_back();
		MakeCircleParticle(particles.back(), pos);
	}
	// 大きい閃光
	for (int32_t count = 0; count < largeFlashData_.count; ++count) {
		particles.emplace_back();
		MakeLargeFlashParticle(particles.back(), pos);
	}
	// 小さい閃光
	for (int32_t count = 0; count < shortFlashData_.count; ++count) {
		particles.emplace_back();
		MakeShortFlashParticle(particles.back(), pos);
	}
	// リング
	for (int32_t count = 0; count < ringData_.count; ++count) {
		particles.emplace_back();
		MakeRingParticle(particles.back(), pos);
	}
	return particles;
}

std::list<ParticleData> ParryEffect::GoodEmission(LWP::Math::Vector3 pos) {
	std::list<ParticleData> particles;

	// 線パーティクル
	for (int32_t count = 0; count < lineParticleData_.count; ++count) {
		particles.emplace_back();
		MakeLineParticle(particles.back(), pos);
	}
	// 小さい閃光
	for (int32_t count = 0; count < shortFlashData_.count; ++count) {
		particles.emplace_back();
		MakeShortFlashParticle(particles.back(), pos);
	}
	return particles;
}

void ParryEffect::CreateJustParticles(Vector3 pos) {
	// 発生地点
	emitterPos_ = pos;

	// パーティクル発生
	particles_.splice(particles_.end(), JustEmission(pos));

	//// 使用するテクスチャ設定
	//for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
	//	switch ((*particleIterator).type) {
	//		// 線
	//	case (int)ParticleType::kLine:
	//		(*particleIterator).plane.materials["Texturematerial"].texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	//		break;
	//		// 円
	//	case (int)ParticleType::kCircle:
	//		(*particleIterator).plane.materials["Texturematerial"].texture = LWP::Resource::LoadTexture("Effect/Particle.png");
	//		break;
	//		// 大きい閃光
	//	case (int)ParticleType::kLargeFlash:
	//		(*particleIterator).plane.materials["Texturematerial"].texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	//		break;
	//		// 小さい閃光
	//	case (int)ParticleType::kShortFlash:
	//		(*particleIterator).plane.materials["Texturematerial"].texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	//		break;
	//		// リング
	//	case (int)ParticleType::kRing:
	//		(*particleIterator).billboard.material.texture = LWP::Resource::LoadTexture("Effect/CircleParticle.png");
	//		break;
	//	}
	//	// ライティングなし
	//	(*particleIterator).plane.materials["Texturematerial"].enableLighting = false;

	//	particleIterator++;
	//}
}

void ParryEffect::CreateGoodParticles(LWP::Math::Vector3 pos) {
	// 発生地点
	emitterPos_ = pos;

	// パーティクル発生
	particles_.splice(particles_.end(), GoodEmission(pos));

	//// 使用するテクスチャ設定
	//for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
	//	switch ((*particleIterator).type) {
	//		// 線
	//	case (int)ParticleType::kLine:
	//		(*particleIterator).plane.materials["Texturematerial"].texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	//		break;
	//		// 小さい閃光
	//	case (int)ParticleType::kShortFlash:
	//		(*particleIterator).plane.materials["Texturematerial"].texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	//		break;
	//	}
	//	// ライティングなし
	//	(*particleIterator).plane.materials["Texturematerial"].enableLighting = false;

	//	particleIterator++;
	//}
}

void ParryEffect::LineParticleUpdate(ParticleData& data) {
	// 重力加速
	data.vel.y += lineParticleData_.acceleration.y * lineParticleData_.multiply;

	// 移動速度から角度算出
	//data.billboard.worldTF.rotation = MathFunc::LookRotation(data.vel);
	//data.plane.worldTF.rotation = MathFunc::LookRotation(data.vel);
	// 移動処理
	data.billboard.worldTF.translation += data.vel;
	//data.plane.worldTF.translation += data.vel;

	// 速度減速
	data.vel = Exponential(data.vel, Vector3{ 0.0f, 0.0f, 0.0f }, 0.01f);

	// 色のイージング
	Vector4 color = {
		LerpF(255.0f, 255.0f, data.currentTime / data.lifeTime),
		LerpF(150.0f, 209.0f, data.currentTime / data.lifeTime),
		LerpF(0.0f, 28.0f, data.currentTime / data.lifeTime),
		LerpF(255.0f, 0.0f, data.currentTime / data.lifeTime)
	};
	data.billboard.material.color = {
		(int)color.x,
		(int)color.y,
		(int)color.z,
		(int)color.w
	};
	//data.plane.materials["Texturematerial"].color = {
	//(int)color.x,
	//(int)color.y,
	//(int)color.z,
	//(int)color.w
	//};
}

void ParryEffect::CircleParticleUpdate(ParticleData& data) {
	if (data.currentTime >= circleParticleEasingEndTime * 60.0f) {

	}
	else {
		// 重力加速
		data.multiply = LWP::Utility::Interpolation::LerpF(circleParticleData_.multiply, 0.5f, data.currentTime / (circleParticleEasingEndTime * 60.0f));
		data.vel.x *= data.multiply;
		data.vel.z *= data.multiply;
	}

	data.vel.y += circleParticleData_.acceleration.y * data.multiply;
	// 移動処理
	data.billboard.worldTF.translation += data.vel;
}

void ParryEffect::LargeFlashUpdate(ParticleData& data) {
	// Y軸のみのビルボード
	Vector3 dist = (followCamera_->GetCamera()->worldTF.GetWorldPosition() - data.billboard.worldTF.GetWorldPosition()).Normalize();
	// Z軸の角度をずらす
	float eulerZ;
	if (data.euler.z <= (float)std::numbers::pi / 2.0f) {
		eulerZ = LerpF(0, LWP::Utility::DegreeToRadian(-5.0f), data.currentTime / data.lifeTime);
	}
	else {
		eulerZ = LerpF(0, LWP::Utility::DegreeToRadian(5.0f), data.currentTime / data.lifeTime);
	}
	data.billboard.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, data.euler.z + eulerZ) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, data.euler.x);
	data.billboard.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, data.euler.y) * data.billboard.worldTF.rotation;

	// 生存時間の半分まで徐々に大きくなる
	if (data.currentTime <= (data.lifeTime / 2.0f)) {
		data.billboard.worldTF.scale = Lerp(Vector3{ 0,0,0 }, maxLargeFlashScale, Easing::OutExpo(data.currentTime / (data.lifeTime / 2.0f)));
	}
	// 徐々に小さくなる
	else {
		data.billboard.worldTF.scale = Lerp(maxLargeFlashScale, Vector3{ 0,0,0 }, Easing::InExpo((data.currentTime - data.lifeTime / 2.0f) / (data.lifeTime / 2.0f)));
	}

	// 色をイージング
	Vector4 color = {
		LerpF(239.0f, 199.0f, data.currentTime / data.lifeTime),
		LerpF(139.0f, 126.0f, data.currentTime / data.lifeTime),
		LerpF(248.0f, 181.0f, data.currentTime / data.lifeTime),
		LerpF(128.0f, 255.0f, data.currentTime / data.lifeTime)
	};
	data.billboard.material.color = {
		(int)color.x,
		(int)color.y,
		(int)color.z,
		(int)color.w
	};
}

void ParryEffect::ShortFlashUpdate(ParticleData& data) {
	// Y軸のみのビルボード
	Vector3 dist = (followCamera_->GetCamera()->worldTF.GetWorldPosition() - data.billboard.worldTF.GetWorldPosition()).Normalize();
	data.billboard.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, data.euler.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, data.euler.x);
	data.billboard.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, data.euler.y) * data.billboard.worldTF.rotation;

	// 生存時間の半分まで徐々に大きくなる
	if (data.currentTime <= (data.lifeTime / 2.0f)) {
		data.billboard.worldTF.scale = Lerp(Vector3{ 0,0,0 }, maxShortFlashScale, Easing::OutExpo(data.currentTime / (data.lifeTime / 2.0f)));
	}
	// 徐々に小さくなる
	else {
		data.billboard.worldTF.scale = Lerp(maxShortFlashScale, Vector3{ 0,0,0 }, Easing::InExpo((data.currentTime - data.lifeTime / 2.0f) / (data.lifeTime / 2.0f)));
	}

	// 色のイージング
	Vector4 color = {
	LerpF(255.0f, 255.0f, data.currentTime / data.lifeTime),
	LerpF(212.0f, 255.0f, data.currentTime / data.lifeTime),
	LerpF(71.0f, 215.0f, data.currentTime / data.lifeTime),
	LerpF(255.0f, 158.0f, data.currentTime / data.lifeTime)
	};
	data.billboard.material.color = {
		(int)color.x,
		(int)color.y,
		(int)color.z,
		(int)color.w
	};
}

void ParryEffect::RingParticleUpdate(ParticleData& data) {
	if (data.currentTime <= data.lifeTime) {
		// 徐々に大きくなる
		data.billboard.worldTF.scale = Lerp(Vector3{ 0,0,0 }, maxRingScale, Easing::OutExpo(data.currentTime / data.lifeTime));
		// 徐々に透明になる
		data.billboard.material.color.A = LerpF(255, 0, Easing::OutExpo(data.currentTime / data.lifeTime));
	}
}

void ParryEffect::SetJsonData(LWP::Utility::JsonIO& json) {
#pragma region Line
	json.BeginGroup("Line");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &lineParticleData_.velocity.max);
	json.AddValue<Vector3>("Min", &lineParticleData_.velocity.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<Vector3>("Max", &lineParticleData_.scale.max);
	json.AddValue<Vector3>("Min", &lineParticleData_.scale.min);
	json.EndGroup();

	json.EndGroup();

	// ケルビン
	json.BeginGroup("Kelvin");
	json.AddValue<float>("Max", &lineParticleKelvin.max);
	json.AddValue<float>("Min", &lineParticleKelvin.min);
	json.EndGroup();

	// 初速
	json.AddValue<Vector3>("First", &lineParticleData_.firstVel);
	// 重力加速度
	json.AddValue<Vector3>("Acceleration", &lineParticleData_.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &lineParticleData_.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &lineParticleData_.maxElapseTime);

	// パーティクルの数
	json.AddValue<int>("Count", &lineParticleData_.count);
	json.EndGroup();
#pragma endregion

#pragma region Circle
	json.BeginGroup("Circle");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &circleParticleData_.velocity.max);
	json.AddValue<Vector3>("Min", &circleParticleData_.velocity.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<Vector3>("Max", &circleParticleData_.scale.max);
	json.AddValue<Vector3>("Min", &circleParticleData_.scale.min);
	json.EndGroup();

	json.EndGroup();

	// ケルビン
	json.BeginGroup("Kelvin");
	json.AddValue<float>("Max", &circleParticleKelvin.max);
	json.AddValue<float>("Min", &circleParticleKelvin.min);
	json.EndGroup();

	// 初速
	json.AddValue<Vector3>("First", &circleParticleData_.firstVel);
	// 重力加速度
	json.AddValue<Vector3>("Acceleration", &circleParticleData_.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &circleParticleData_.multiply);

	json.AddValue<float>("VelocityEaseEndTime", &circleParticleEasingEndTime);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &circleParticleData_.maxElapseTime);

	// パーティクルの数
	json.AddValue<int>("Count", &circleParticleData_.count);
	json.EndGroup();
#pragma endregion

#pragma region LargeFlash
	json.BeginGroup("LargeFlash");
	// 最大サイズ
	json.AddValue<Vector3>("MaxScale", &maxLargeFlashScale);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &largeFlashData_.maxElapseTime);

	// パーティクルの数
	json.AddValue<int>("Count", &largeFlashData_.count);
	json.EndGroup();
#pragma endregion

#pragma region ShortFlash
	json.BeginGroup("ShortFlash");
	// 最大サイズ
	json.AddValue<Vector3>("MaxScale", &maxShortFlashScale);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &shortFlashData_.maxElapseTime);

	// パーティクルの数
	json.AddValue<int>("Count", &shortFlashData_.count);
	json.EndGroup();
#pragma endregion

#pragma region Ring
	json.BeginGroup("Ring");
	// 最大サイズ
	json.AddValue<Vector3>("MaxScale", &maxRingScale);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &ringData_.maxElapseTime);

	// パーティクルの数
	json.AddValue<int>("Count", &ringData_.count);
	json.EndGroup();
#pragma endregion
}
