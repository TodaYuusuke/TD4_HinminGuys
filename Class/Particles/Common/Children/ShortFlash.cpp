#include "ShortFlash.h"
#include "../../../Camera/FollowCamera.h"
#include "../../../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

ShortFlash::ShortFlash(FollowCamera* followCamera, const std::string& texName) {
	followCamera_ = followCamera;
	texName_ = texName;
}

void ShortFlash::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void ShortFlash::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kShortFlash;
	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	// 角度(オイラー角で算出)
	Vector3 dist = (followCamera_->GetCamera()->worldTF.GetWorldPosition() - plane_.worldTF.GetWorldPosition()).Normalize();
	Vector3 radian = {
		0.0f,
		0.0f,
		LWP::Utility::Random::GenerateFloat(LWP::Utility::DegreeToRadian(0.0f), LWP::Utility::DegreeToRadian(180.0f))
	};
	particleData_.euler = radian;
	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;

#pragma region 平面に反映
	// 黄色に変更
	plane_.material.color = { 255,212,71,255 };
	// 座標
	plane_.worldTF.translation = pos;
	// 大きさ
	plane_.worldTF.scale = { 0,0,0 };
	// 角度
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, radian.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, radian.x);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, radian.y) * plane_.worldTF.rotation;
#pragma endregion
}

void ShortFlash::UpdateParticle() {
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, particleData_.euler.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, particleData_.euler.x);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, particleData_.euler.y) * plane_.worldTF.rotation;

	// 生存時間の半分まで徐々に大きくなる
	if (particleData_.currentTime <= (particleData_.lifeTime / 2.0f)) {
		plane_.worldTF.scale = Lerp(Vector3{ 0,0,0 }, jsonData_.maxScale, Easing::OutExpo(particleData_.currentTime / (particleData_.lifeTime / 2.0f)));
	}
	// 徐々に小さくなる
	else {
		plane_.worldTF.scale = Lerp(jsonData_.maxScale, Vector3{ 0,0,0 }, Easing::InExpo((particleData_.currentTime - particleData_.lifeTime / 2.0f) / (particleData_.lifeTime / 2.0f)));
	}

	// 色のイージング
	Vector4 color = {
	LerpF(255.0f, 255.0f, particleData_.currentTime / particleData_.lifeTime),
	LerpF(212.0f, 255.0f, particleData_.currentTime / particleData_.lifeTime),
	LerpF(71.0f, 215.0f, particleData_.currentTime / particleData_.lifeTime),
	LerpF(255.0f, 158.0f, particleData_.currentTime / particleData_.lifeTime)
	};
	plane_.material.color = {
		(int)color.x,
		(int)color.y,
		(int)color.z,
		(int)color.w
	};
}