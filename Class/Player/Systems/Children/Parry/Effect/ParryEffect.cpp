#include "ParryEffect.h"
#include "../../../../../Camera/FollowCamera.h"
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
	for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		// 生存時間が過ぎたら処理を行わない
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		LWP::Math::Quaternion q{};
		// 各パーティクルの更新処理
		switch ((*particleIterator).type) {
			// 線
		case ParticleType::kLine: {
			// 重力加速
			(*particleIterator).vel.y += lineParticleData_.acceleration.y * lineParticleData_.multiply;
			// 移動速度から角度算出
			q = LookRotation((*particleIterator).vel);
			//q = StretchedBillboard((*particleIterator).plane.worldTF.GetWorldPosition(), (*particleIterator).vel, followCamera_->GetCamera()->worldTF.GetWorldPosition());
			(*particleIterator).plane.worldTF.rotation = q;
			// スケール（Z軸方向に速度依存）
			float speed = (*particleIterator).vel.Length();
			float stretchFactor = 0.2f;
			Vector3 scaleVec = { 1.0f, 1.0f, speed * stretchFactor };
			//(*particleIterator).plane.worldTF.scale.z = scaleVec.z;
			// 移動処理
			(*particleIterator).plane.worldTF.translation += (*particleIterator).vel;
		}
				break;

			// 円
		case ParticleType::kCircle:
			if ((*particleIterator).currentTime >= circleParticleEasingEndTime * 60.0f) {

			}
			else {
				// 重力加速
				(*particleIterator).multiply = LWP::Utility::Interpolation::LerpF(circleParticleData_.multiply, 0.5f, (*particleIterator).currentTime / (circleParticleEasingEndTime * 60.0f));
				(*particleIterator).vel.x *= (*particleIterator).multiply;
				(*particleIterator).vel.z *= (*particleIterator).multiply;
			}

			(*particleIterator).vel.y += circleParticleData_.acceleration.y * (*particleIterator).multiply;
			// 移動処理
			(*particleIterator).billboard.worldTF.translation += (*particleIterator).vel;
			break;
		}

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

ParryEffect::ParticleData ParryEffect::MakeLineParticle(Vector3 pos) {
	ParticleData particle;

	// パーティクルの種類
	particle.type = ParticleType::kLine;

	// ビルボード生成(非表示)
	particle.billboard.anchorPoint = { 0.5f, 0.5f };
	particle.billboard.isActive = false;
	particle.billboard.material.enableLighting = false;
	particle.billboard.material.texture = LWP::Resource::LoadTexture("Effect/Particle.png");
	particle.billboard.Init();
	// 平面生成
	particle.plane.LoadShortPath("BothPlane.obj");

	// 色
	float kelvin = LWP::Utility::Random::GenerateFloat(lineParticleKelvin.min, lineParticleKelvin.max);
	particle.plane.materials["Texturematerial"].color = LWP::Utility::Color::KelvinToRGB(kelvin);

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(lineParticleData_.velocity.min, lineParticleData_.velocity.max);
	particle.vel = vel;
	particle.vel.y += lineParticleData_.firstVel.y;

	// 座標
	particle.plane.worldTF.translation = pos;
	// 大きさ
	Vector3 scale = LWP::Utility::Random::GenerateVector3(lineParticleData_.scale.min, lineParticleData_.scale.max);
	particle.plane.worldTF.scale = scale;

	// 生存可能時間
	particle.lifeTime = lineParticleData_.maxElapseTime * 60.0f;
	particle.currentTime = 0;

	return particle;
}

ParryEffect::ParticleData ParryEffect::MakeCircleParticle(LWP::Math::Vector3 pos) {
	ParticleData particle;

	// パーティクルの種類
	particle.type = ParticleType::kCircle;

	// ビルボード生成
	particle.billboard.anchorPoint = { 0.5f, 0.5f };
	particle.billboard.material.enableLighting = false;
	particle.billboard.material.texture = LWP::Resource::LoadTexture("Effect/Particle.png");
	particle.billboard.Init();
	// 平面生成(非表示)
	particle.plane.LoadShortPath("BothPlane.obj");
	particle.plane.isActive = false;

	// 色
	//float kelvin = LWP::Utility::Random::GenerateFloat(lineParticleKelvin.min, lineParticleKelvin.max);
	//particle.plane.materials[""].color = LWP::Utility::Color::KelvinToRGB(kelvin);

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

	return particle;
}

std::list<ParryEffect::ParticleData> ParryEffect::Emission(LWP::Math::Vector3 pos) {
	std::list<ParticleData> particles;

	// 線パーティクル
	for (int32_t count = 0; count < lineParticleData_.count; ++count) {
		particles.push_back(MakeLineParticle(pos));
	}
	// 円パーティクル
	for (int32_t count = 0; count < circleParticleData_.count; ++count) {
		particles.push_back(MakeCircleParticle(pos));
	}
	return particles;
}

Quaternion ParryEffect::LookRotation(const Vector3& dirVec) {
	// 向かせたい方向ベクトル
	Vector3 dir = dirVec.Normalize();
	// デフォルトの方向ベクトル
	Vector3 norm = { 0,0,1 };
	// 二つの方向ベクトルから角度算出
	float theta = std::acosf(Vector3::Dot(norm, dir)) / 2.0f;
	Vector3 cross = Vector3::Cross(norm, dir).Normalize();

	// 計算結果
	Quaternion q = {
		cross.x * std::sin(theta),
		cross.y * std::sin(theta),
		cross.z * std::sin(theta),
		std::cos(theta)
	};
	return q;
}

LWP::Math::Quaternion ParryEffect::HorizontalBillboard(const LWP::Math::Vector3& targetPos) {
	Vector3 dir = followCamera_->GetCamera()->worldTF.GetWorldPosition() - targetPos;
	dir.y = 0;  // 水平成分だけ使う
	Quaternion result = LookRotation(dir);

	return result;
}

LWP::Math::Quaternion ParryEffect::StretchedBillboard(const LWP::Math::Vector3& targetPos, const LWP::Math::Vector3& vel, const LWP::Math::Vector3& cameraPos) {
	Vector3 pos = targetPos;
	Vector3 cam = cameraPos;

	// forward = 速度方向（Z+）
	Vector3 forward = vel.Normalize();

	// camera direction（Billboard基準）
	Vector3 toCam = (cam - pos).Normalize();

	// right = forward × toCam
	Vector3 right = Vector3::Cross(forward, toCam).Normalize();

	// up = right × forward
	Vector3 up = Vector3::Cross(right, forward);

	// 回転行列を構築（列ベース）
	Matrix4x4 rotMatrix = {
		//right,    // X
		//up,       // Y
		//forward,  // Z
		//XMVectorSet(0, 0, 0, 1)
	};
	rotMatrix.m[0][0] = right.x;
	rotMatrix.m[0][1] = right.y;
	rotMatrix.m[0][2] = right.z;
	rotMatrix.m[0][3] = 0.0f;

	rotMatrix.m[1][0] = up.x;
	rotMatrix.m[1][1] = up.y;
	rotMatrix.m[1][2] = up.z;
	rotMatrix.m[1][3] = 0.0f;

	rotMatrix.m[2][0] = forward.x;
	rotMatrix.m[2][1] = forward.y;
	rotMatrix.m[2][2] = forward.z;
	rotMatrix.m[2][3] = 0.0f;

	rotMatrix.m[3][0] = 0.0f;
	rotMatrix.m[3][1] = 0.0f;
	rotMatrix.m[3][2] = 0.0f;
	rotMatrix.m[3][3] = 1.0f;


	// クォータニオンへ変換
	Quaternion q = QuaternionRotateMatrix(rotMatrix);

	return q;
}

LWP::Math::Quaternion ParryEffect::QuaternionRotateMatrix(const LWP::Math::Matrix4x4& m) {
	Quaternion q;
	float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];

	if (trace > 0.0f) {
		float s = sqrtf(trace + 1.0f) * 2.0f;
		q.w = 0.25f * s;
		q.x = (m.m[2][1] - m.m[1][2]) / s;
		q.y = (m.m[0][2] - m.m[2][0]) / s;
		q.z = (m.m[1][0] - m.m[0][1]) / s;
	}
	else {
		if (m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2]) {
			float s = sqrtf(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]) * 2.0f;
			q.w = (m.m[2][1] - m.m[1][2]) / s;
			q.x = 0.25f * s;
			q.y = (m.m[0][1] + m.m[1][0]) / s;
			q.z = (m.m[0][2] + m.m[2][0]) / s;
		}
		else if (m.m[1][1] > m.m[2][2]) {
			float s = sqrtf(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]) * 2.0f;
			q.w = (m.m[0][2] - m.m[2][0]) / s;
			q.x = (m.m[0][1] + m.m[1][0]) / s;
			q.y = 0.25f * s;
			q.z = (m.m[1][2] + m.m[2][1]) / s;
		}
		else {
			float s = sqrtf(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]) * 2.0f;
			q.w = (m.m[1][0] - m.m[0][1]) / s;
			q.x = (m.m[0][2] + m.m[2][0]) / s;
			q.y = (m.m[1][2] + m.m[2][1]) / s;
			q.z = 0.25f * s;
		}
	}

	return q;
}

float ParryEffect::ExponentialInterpolateF(const float& current, const float& target, float damping) {
	float factor = 1.0f - std::exp(-damping);
	return current + (target - current) * factor;
}

float ParryEffect::GetRotationAngleFromMatrix(const LWP::Math::Matrix4x4& m) {
	// 回転部分のトレースを計算
	float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];

	// トレースから cosθ を求める
	float cosTheta = (trace - 1.0f) * 0.5f;

	// 安全のため -1〜1 にクランプ
	cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

	// θ = arccos( (trace - 1) / 2 )
	float theta = std::acos(cosTheta);

	return theta; // ラジアン単位
}

LWP::Math::Quaternion ParryEffect::QuaternionFromMatrix(const LWP::Math::Matrix4x4& m) {
	float theta = GetRotationAngleFromMatrix(m); // ラジアン
	float sinTheta = std::sin(theta);

	// 小さすぎるときは無回転扱い
	if (std::abs(sinTheta) < 1e-6f) {
		return { 0, 0, 0, 1 }; // 単位クォータニオン
	}

	// 回転軸（normalized）
	float ux = (m.m[2][1] - m.m[1][2]) / (2.0f * sinTheta);
	float uy = (m.m[0][2] - m.m[2][0]) / (2.0f * sinTheta);
	float uz = (m.m[1][0] - m.m[0][1]) / (2.0f * sinTheta);

	float halfTheta = theta * 0.5f;
	float sinHalf = std::sin(halfTheta);
	float cosHalf = std::cos(halfTheta);

	Quaternion q;
	q.x = ux * sinHalf;
	q.y = uy * sinHalf;
	q.z = uz * sinHalf;
	q.w = cosHalf;

	return q;
}

void ParryEffect::Create(Vector3 pos) {
	particles_.splice(particles_.end(), Emission(pos));

	for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		(*particleIterator).plane.materials["Texturematerial"].enableLighting = false;
		(*particleIterator).plane.materials["Texturematerial"].texture = LWP::Resource::LoadTexture("Effect/Spark.png");
		particleIterator++;
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
}
