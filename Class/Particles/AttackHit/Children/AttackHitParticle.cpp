#include "AttackHitParticle.h"
#include "../../../Player/Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

AttackHitParticle::AttackHitParticle(const std::string& texName) {
	texName_ = texName;
}

void AttackHitParticle::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void AttackHitParticle::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kLine;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(jsonData_.velLimit.min, jsonData_.velLimit.max);
	particleData_.vel = vel + jsonData_.firstVel;
	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;

#pragma region 平面に反映
	// 色
	plane_.material.color = jsonData_.color;
	// 座標
	plane_.worldTF.translation = pos;
	// 大きさ
	float scale = LWP::Utility::Random::GenerateFloat(jsonData_.scaleLimit.min, jsonData_.scaleLimit.max);
	randomScale_ = {
		scale,
		scale,
		scale
	};
	plane_.worldTF.scale = randomScale_;
#pragma endregion
}

void AttackHitParticle::UpdateParticle() {
	// 重力加速
	particleData_.vel.y += jsonData_.acceleration * jsonData_.multiply;

	// 移動処理
	plane_.worldTF.translation += particleData_.vel;

	if (plane_.worldTF.translation.y <= 0.0f) {
		particleData_.vel.x *= 0.8f;
		particleData_.vel.y *= -0.8f;
		particleData_.vel.z *= 0.8f;
	}

	// 色のイージング
	int alpha = LerpF((float)jsonData_.color.A, 0.0f, particleData_.currentTime / particleData_.lifeTime);
	plane_.material.color.A = alpha;
}
