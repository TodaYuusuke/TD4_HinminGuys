#include "Spark.h"
#include "../../../Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

Spark::Spark(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;
}

void Spark::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void Spark::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kLine;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	plane_.material.uvTransform.rotation = { 0.0f, 0.0f, 3.14f / 2.0f };

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(jsonData_.velLimit.min, jsonData_.velLimit.max);
	particleData_.vel = vel;
	particleData_.vel.y += jsonData_.firstVel.y;
	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;

#pragma region 平面に反映
	// 色
	float kelvin = LWP::Utility::Random::GenerateFloat(jsonData_.kelvinLimit.min, jsonData_.kelvinLimit.max);
	plane_.material.color = LWP::Utility::Color::KelvinToRGB(kelvin);
	// 座標
	plane_.worldTF.translation = pos;
	// 大きさ
	Vector3 scale = LWP::Utility::Random::GenerateVector3(jsonData_.scaleLimit.min, jsonData_.scaleLimit.max);
	plane_.worldTF.scale = scale;
	// 速度
	plane_.velocity = particleData_.vel;
#pragma endregion
}

void Spark::UpdateParticle() {
	// 重力加速
	particleData_.vel.y += jsonData_.acceleration.y * jsonData_.multiply;

	// 移動処理
	plane_.worldTF.translation += particleData_.vel;

	// 速度減速
	particleData_.vel = Exponential(particleData_.vel, Vector3{ 0.0f, 0.0f, 0.0f }, 0.01f);

	// stretchビルボードの計算のために速度を代入
	plane_.velocity = particleData_.vel;

	// 色のイージング
	Vector4 color = {
		LerpF(255.0f, 255.0f, particleData_.currentTime / particleData_.lifeTime),
		LerpF(150.0f, 209.0f, particleData_.currentTime / particleData_.lifeTime),
		LerpF(0.0f, 28.0f, particleData_.currentTime / particleData_.lifeTime),
		LerpF(255.0f, 0.0f, particleData_.currentTime / particleData_.lifeTime)
	};
	plane_.material.color = {
		(int)color.x,
		(int)color.y,
		(int)color.z,
		(int)color.w
	};
}
