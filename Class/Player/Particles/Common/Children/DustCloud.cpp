#include "DustCloud.h"
#include "../../../Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

DustCloud::DustCloud(const std::string& texName) {
	texName_ = texName;
}

void DustCloud::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void DustCloud::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kCircle;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.enableLighting = false;
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(jsonData_.velLimit.min, jsonData_.velLimit.max);
	Vector3 rot = { 0.0f, shotRotate_.y, 0.0f };
	Vector3 dir = Vector3{ 0.0f, 0.0f, 1.0f } * Math::Matrix4x4::CreateRotateXYZMatrix(rot);
	particleData_.vel = vel + dir * jsonData_.multiply;
	particleData_.vel.y += jsonData_.firstVel.y;

	// 座標
	plane_.worldTF.translation = pos;
	// 大きさ
	float scale = LWP::Utility::Random::GenerateFloat(jsonData_.scaleLimit.min, jsonData_.scaleLimit.max);
	plane_.worldTF.scale = {
		scale,
		scale,
		scale
	};

	// 色
	int white = LWP::Utility::Random::GenerateInt(jsonData_.whiteLimit.min, jsonData_.whiteLimit.max);
	int alpha = LWP::Utility::Random::GenerateInt(jsonData_.alphaLimit.min, jsonData_.alphaLimit.max);

	plane_.material.color = { white,white,white,alpha };

	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;
}

void DustCloud::UpdateParticle() {
	particleData_.vel.y += jsonData_.acceleration;
	// 移動処理
	plane_.worldTF.translation += particleData_.vel;

	// 速度減衰
	particleData_.vel = Exponential(particleData_.vel, Vector3{ 0,0,0 }, jsonData_.dampingRate);
}
