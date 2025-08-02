#include "RhombusParticle.h"
#include "../../../Camera/FollowCamera.h"
#include "../../../Player/Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

RhombusParticle::RhombusParticle(FollowCamera* followCamera, const std::string& texName) {
	followCamera_ = followCamera;
	texName_ = texName;
}

void RhombusParticle::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void RhombusParticle::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kCircle;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.enableLighting = false;
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(jsonData_.velLimit.min, jsonData_.velLimit.max);
	Vector3 dir = Vector3{ 0.0f, 0.0f, 1.0f } * Math::Matrix4x4::CreateRotateXYZMatrix(shotRotate_ + followCamera_->GetRadian());
	particleData_.vel = vel + dir * jsonData_.multiply;
	particleData_.vel.y += jsonData_.firstVel.y;

	// 座標
	Vector3 range = Vector3{ 0.0f, 0.0f, jsonData_.creaateRange } * Math::Matrix4x4::CreateRotateXYZMatrix(shotRotate_ + followCamera_->GetRadian());
	plane_.worldTF.translation = pos + range;
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

void RhombusParticle::UpdateParticle() {
	particleData_.vel.y += jsonData_.acceleration;
	// 移動処理
	plane_.worldTF.translation += particleData_.vel * jsonData_.multiply;

	// サイズのイージング
	plane_.worldTF.scale = Lerp(randomScale_, jsonData_.maxScale, Easing::OutExpo(particleData_.currentTime / particleData_.lifeTime));
	// 速度減衰
	//particleData_.vel = Exponential(particleData_.vel, Vector3{ 0,0,0 }, jsonData_.dampingRate);
}

