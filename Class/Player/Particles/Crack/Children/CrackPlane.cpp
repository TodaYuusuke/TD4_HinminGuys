#include "CrackPlane.h"
#include <numbers>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

CrackPlane::CrackPlane(const std::string& texName) {
	texName_ = texName;
}

void CrackPlane::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void CrackPlane::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kCrackPlane;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	// 座標
	plane_.worldTF.translation = pos;
	plane_.worldTF.translation.y = 0.01f;
	// 大きさ
	plane_.worldTF.scale = jsonData_.scale;
	// 角度
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, 0.0f) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, (float)std::numbers::pi / 2.0f);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, 0.0f) * plane_.worldTF.rotation;

	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;
}

void CrackPlane::UpdateParticle() {
	if (particleData_.currentTime <= particleData_.lifeTime) {
		// 徐々に透明になる
		plane_.material.color.A = LerpF(255, 0, Easing::InExpo(particleData_.currentTime / particleData_.lifeTime));
	}
}
