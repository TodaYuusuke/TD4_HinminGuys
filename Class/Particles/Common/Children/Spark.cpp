#include "Spark.h"
#include "../../../Player/Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

Spark::Spark(const std::string& texName) {
	texName_ = texName;
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
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(jsonData_.velLimit.min, jsonData_.velLimit.max);
	particleData_.vel = vel + particleData_.dirVec;
	particleData_.vel.y += jsonData_.firstVel.y;
	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;
	// 大きさ
	Vector3 scale = LWP::Utility::Random::GenerateVector3(jsonData_.scaleLimit.min, jsonData_.scaleLimit.max);
	maxScale_ = scale;

#pragma region 平面に反映
	// 色
	plane_.material.color = jsonData_.color;
	// 座標
	plane_.worldTF.translation = pos + jsonData_.createRange * vel.Normalize();
	// 大きさ
	plane_.worldTF.scale = maxScale_;
	// 速度
	plane_.velocity = particleData_.vel + (jsonData_.createRange * vel.Normalize()).Normalize();
#pragma endregion
}

void Spark::UpdateParticle() {
	// 重力加速
	particleData_.vel.y += jsonData_.acceleration.y * jsonData_.multiply;

	// 移動処理
	plane_.worldTF.translation += particleData_.vel;

	// 速度減速
	//particleData_.vel = Exponential(particleData_.vel, Vector3{ 0.0f, 0.0f, 0.0f }, 0.01f);

	// stretchビルボードの計算のために速度を代入
	plane_.velocity = particleData_.vel * 100.0f + particleData_.dirVec;

	// スケールのイージング
	Vector3 scale = Lerp(maxScale_, Vector3{ 0.0f,0.0f,0.0f }, particleData_.currentTime / particleData_.lifeTime);
	plane_.worldTF.scale = scale;
	// 色のイージング
	int alpha = LerpF((float)jsonData_.color.A, 0.0f, particleData_.currentTime / particleData_.lifeTime);
	plane_.material.color.A = alpha;
}
