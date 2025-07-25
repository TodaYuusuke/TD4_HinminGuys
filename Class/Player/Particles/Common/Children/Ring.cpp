#include "Ring.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

Ring::Ring(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;
}

void Ring::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void Ring::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kRing;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture("Effect/CircleParticle.png");

	// 座標
	plane_.worldTF.translation = pos;
	// 大きさ
	plane_.worldTF.scale = { 0,0,0 };

	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;
}

void Ring::UpdateParticle() {
	if (particleData_.currentTime <= particleData_.lifeTime) {
		// 徐々に大きくなる
		plane_.worldTF.scale = Lerp(Vector3{ 0,0,0 }, jsonData_.maxScale, Easing::OutExpo(particleData_.currentTime / particleData_.lifeTime));
		// 徐々に透明になる
		plane_.material.color.A = LerpF(255, 0, Easing::OutExpo(particleData_.currentTime / particleData_.lifeTime));
	}
}
