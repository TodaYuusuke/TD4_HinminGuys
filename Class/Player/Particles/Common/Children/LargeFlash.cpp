#include "LargeFlash.h"
#include "../../../../Camera/FollowCamera.h"
#include "../../../../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

LargeFlash::LargeFlash(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;
}

void LargeFlash::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void LargeFlash::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//data.type = (int)ParticleType::kLargeFlash;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture("Effect/Spark.png");
	plane_.worldTF.rotation *= LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, 0.2f);

	// 角度
	Vector3 dist = (followCamera_->GetCamera()->worldTF.GetWorldPosition() - plane_.worldTF.GetWorldPosition()).Normalize();
	Vector3 radian = {
		0.0f,
		std::atan2(dist.x, dist.z) + LWP::Utility::Random::GenerateFloat(LWP::Utility::DegreeToRadian(-30.0f),LWP::Utility::DegreeToRadian(30.0f)),
		LWP::Utility::Random::GenerateFloat(LWP::Utility::DegreeToRadian(60.0f), LWP::Utility::DegreeToRadian(120.0f))
	};
	particleData_.euler = radian;
	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;

	// アヤメ色に変更
	plane_.material.color = { 199, 126, 181, 122 };

	// 座標
	plane_.worldTF.translation = pos;
	// 大きさ
	plane_.worldTF.scale = { 0,0,0 };

	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, radian.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, radian.x);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, radian.y) * plane_.worldTF.rotation;
}

void LargeFlash::UpdateParticle() {
	// Z軸の角度をずらす
	float eulerZ;
	if (particleData_.euler.z <= (float)std::numbers::pi / 2.0f) {
		eulerZ = LerpF(0, LWP::Utility::DegreeToRadian(-5.0f), particleData_.currentTime / particleData_.lifeTime);
	}
	else {
		eulerZ = LerpF(0, LWP::Utility::DegreeToRadian(5.0f), particleData_.currentTime / particleData_.lifeTime);
	}
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, particleData_.euler.z + eulerZ) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, particleData_.euler.x);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, particleData_.euler.y) * plane_.worldTF.rotation;

	// 生存時間の半分まで徐々に大きくなる
	if (particleData_.currentTime <= (particleData_.lifeTime / 2.0f)) {
		plane_.worldTF.scale = Lerp(Vector3{ 0,0,0 }, jsonData_.maxScale, Easing::OutExpo(particleData_.currentTime / (particleData_.lifeTime / 2.0f)));
	}
	// 徐々に小さくなる
	else {
		plane_.worldTF.scale = Lerp(jsonData_.maxScale, Vector3{ 0,0,0 }, Easing::InExpo((particleData_.currentTime - particleData_.lifeTime / 2.0f) / (particleData_.lifeTime / 2.0f)));
	}

	// 色をイージング
	Vector4 color = {
		LerpF(239.0f, 199.0f, particleData_.currentTime / particleData_.lifeTime),
		LerpF(139.0f, 126.0f, particleData_.currentTime / particleData_.lifeTime),
		LerpF(248.0f, 181.0f, particleData_.currentTime / particleData_.lifeTime),
		LerpF(128.0f, 255.0f, particleData_.currentTime / particleData_.lifeTime)
	};
	plane_.material.color = {
		(int)color.x,
		(int)color.y,
		(int)color.z,
		(int)color.w
	};
}