#include "NoticeCircle.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

NoticeCircle::NoticeCircle() {}

void NoticeCircle::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void NoticeCircle::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kNoticeCircle;

	// ビルボード生成
	planes_[0].material.texture = LWP::Resource::LoadTexture("Effect/CircleParticle.png");
	planes_[1].material.texture = LWP::Resource::LoadTexture("Effect/Circle.png");
	for (int i = 0; i < planes_.size(); i++) {
		planes_[i].anchorPoint = { 0.5f, 0.5f };
		// 色
		planes_[i].material.color = jsonData_.color;
		// 座標
		planes_[i].worldTF.translation = pos;
		// 大きさ
		planes_[i].worldTF.scale = { 0,0,0 };
	}

	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;
}

void NoticeCircle::UpdateParticle() {
	// 縮む
	if (particleData_.currentTime <= jsonData_.minScaleTime * 60.0f) {
		planes_[0].worldTF.scale = Lerp(jsonData_.minScale, Vector3{ 0,0,0 }, (particleData_.currentTime / (jsonData_.minScaleTime * 60.0f)));
	}
	// 拡大
	else if (particleData_.currentTime >= jsonData_.minScaleTime * 60.0f && 
			 particleData_.currentTime <= jsonData_.minScaleTime * 60.0f + jsonData_.maxScaleTime * 60.0f) {
		planes_[0].worldTF.scale = Lerp(Vector3{ 0,0,0 }, jsonData_.maxScale, Easing::OutExpo((particleData_.currentTime - jsonData_.minScaleTime * 60.0f) / (jsonData_.minScaleTime * 60.0f)));
		planes_[1].worldTF.scale = Lerp(Vector3{ 0,0,0 }, 0.6f * jsonData_.maxScale, Easing::InExpo((particleData_.currentTime - jsonData_.minScaleTime * 60.0f) / (jsonData_.minScaleTime * 60.0f)));

		// 徐々に透明になる
		for (int i = 0; i < planes_.size(); i++) {
			planes_[i].material.color.A = (unsigned char)LerpF(255, 0, Easing::OutExpo((particleData_.currentTime - jsonData_.minScaleTime * 60.0f) / (jsonData_.maxScaleTime * 60.0f)));
		}
	}
}

