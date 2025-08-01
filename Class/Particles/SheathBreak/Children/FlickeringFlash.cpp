#include "FlickeringFlash.h"
#include "../../../Camera/FollowCamera.h"
#include "../../../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

FlickeringFlash::FlickeringFlash(FollowCamera* followCamera, const std::string& texName) {
	followCamera_ = followCamera;
	texName_ = texName;
}

void FlickeringFlash::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void FlickeringFlash::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//data.type = (int)ParticleType::kFlickeringFlash;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);
	plane_.worldTF.rotation *= LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, 0.2f);

	// 角度
	Vector3 dist = (followCamera_->GetCamera()->worldTF.GetWorldPosition() - plane_.worldTF.GetWorldPosition()).Normalize();
	Vector3 radian = {
		0.0f,
		std::atan2(dist.x, dist.z) + LWP::Utility::Random::GenerateFloat(LWP::Utility::DegreeToRadian(-30.0f),LWP::Utility::DegreeToRadian(30.0f)),
		LWP::Utility::Random::GenerateFloat(LWP::Utility::DegreeToRadian(60.0f), LWP::Utility::DegreeToRadian(120.0f)) + 3.14f / 2.0f
	};
	particleData_.euler += radian;
	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;

	// アヤメ色に変更
	plane_.material.color = { 199, 126, 181, 122 };

	// 座標
	plane_.worldTF.translation = pos;
	// 大きさ
	plane_.worldTF.scale = particleData_.scale;
	// 角度
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, particleData_.euler.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, particleData_.euler.x);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, particleData_.euler.y) * plane_.worldTF.rotation;
}

void FlickeringFlash::UpdateParticle() {
	// Z軸の角度をずらす
	float eulerZ;
	if (particleData_.euler.z <= (float)std::numbers::pi / 2.0f) {
		eulerZ = LerpF(0, LWP::Utility::DegreeToRadian(-jsonData_.maxRotDegree.z), particleData_.currentTime / particleData_.lifeTime);
	}
	else {
		eulerZ = LerpF(0, LWP::Utility::DegreeToRadian(jsonData_.maxRotDegree.z), particleData_.currentTime / particleData_.lifeTime);
	}
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, particleData_.euler.z + eulerZ) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, particleData_.euler.x);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, particleData_.euler.y) * plane_.worldTF.rotation;

	// 徐々に大きくなる
	if (particleData_.currentTime <= jsonData_.maxScaleTime) {
		plane_.worldTF.scale = Lerp(particleData_.scale, jsonData_.maxScale, Easing::OutExpo(particleData_.currentTime / jsonData_.maxScaleTime));
	}
	// 徐々に小さくなる
	else if (particleData_.currentTime <= jsonData_.maxScaleTime + jsonData_.minScaleTime) {
		plane_.worldTF.scale = Lerp(jsonData_.maxScale, jsonData_.minScale, Easing::InExpo(particleData_.currentTime - jsonData_.maxScaleTime / jsonData_.minScaleTime));
	}
}