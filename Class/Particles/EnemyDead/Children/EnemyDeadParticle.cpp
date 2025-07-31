#include "EnemyDeadParticle.h"
#include "../../../Player/Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

EnemyDeadParticle::EnemyDeadParticle(const std::string& texName) {
	texName_ = texName;
}

void EnemyDeadParticle::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void EnemyDeadParticle::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kCircle;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.enableLighting = false;
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	// 速度
	particleData_.vel = LWP::Utility::Random::GenerateVector3(jsonData_.velLimit.min, jsonData_.velLimit.max);

	// 座標
	plane_.worldTF.translation = pos;
	// 大きさ
	float scale = LWP::Utility::Random::GenerateFloat(jsonData_.scaleLimit.min, jsonData_.scaleLimit.max);
	randomScale_ = { scale, scale, scale };
	plane_.worldTF.scale = randomScale_;

	// 色
	plane_.material.color = jsonData_.color;

	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;
}

void EnemyDeadParticle::UpdateParticle() {
	particleData_.vel.y += jsonData_.acceleration;
	// 移動処理
	plane_.worldTF.translation += particleData_.vel * jsonData_.multiply;

	plane_.worldTF.scale = Lerp(randomScale_, randomScale_ * 2.0f, Easing::InExpo(particleData_.currentTime / particleData_.lifeTime));

	// 色のイージング
	int alpha = (int)LerpF((float)jsonData_.color.A, 0.0f, Easing::InExpo(particleData_.currentTime / particleData_.lifeTime));
	plane_.material.color.A = (unsigned char)alpha;
}
