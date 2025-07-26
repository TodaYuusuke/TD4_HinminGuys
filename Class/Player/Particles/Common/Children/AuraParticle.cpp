#include "AuraParticle.h"
#include "../../../Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

AuraParticle::AuraParticle(Player* player, FollowCamera* followCamera, const std::string& texName) {
	player_ = player;
	followCamera_ = followCamera;
	texName_ = texName;
}

void AuraParticle::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void AuraParticle::Create(const LWP::Math::Vector3& pos) {
	//emitterPos_ = pos;
	// パーティクルの種類
	//particle.type = (int)ParticleType::kLine;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	plane_.worldTF.translation = (emitterPos_->GetWorldPosition());

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(jsonData_.velLimit.min, jsonData_.velLimit.max);
	particleData_.vel = vel;
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
	plane_.worldTF.scale = {
		scale,
		scale,
		scale
	};
#pragma endregion
}

void AuraParticle::UpdateParticle() {
	// 重力加速
	particleData_.vel.y += jsonData_.acceleration * jsonData_.multiply;

	// 移動処理
	localPos_ += particleData_.vel;
	plane_.worldTF.translation = emitterPos_->GetWorldPosition() + localPos_;

	// 速度減速
	particleData_.vel.x = MathFunc::ExponentialInterpolateF(particleData_.vel.x, 0.0f, 0.01f);
	particleData_.vel.z = MathFunc::ExponentialInterpolateF(particleData_.vel.z, 0.0f, 0.01f);

	// 色のイージング
	int alpha = (int)LerpF((float)jsonData_.color.A, 0.0f, particleData_.currentTime / particleData_.lifeTime);
	plane_.material.color.A = alpha;
}
