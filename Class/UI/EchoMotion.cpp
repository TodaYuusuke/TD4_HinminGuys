#include "EchoMotion.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

EchoMotion::EchoMotion(std::string fileName) {
	afterimage_.material.texture = LWP::Resource::LoadTexture(fileName);
	Initialize();
}

EchoMotion::EchoMotion(LWP::Resource::Texture texture) {
	afterimage_.material.texture = texture;
	Initialize();
}

void EchoMotion::Initialize() {
	afterimage_.isActive = true;
	afterimage_.anchorPoint = { 0.5f, 0.5f };
	currentFrame_ = 0.0f;
}

void EchoMotion::Update() {
	afterimage_.isActive = true;
	// 徐々に大きくなる
	afterimage_.worldTF.scale = Lerp(startScale_, endScale_, Utility::Easing::OutExpo(currentFrame_ / endFrame_));
	// 透明度
	afterimage_.material.color.A = (int)LerpF(startAlpha_, 0.0f, Utility::Easing::OutExpo(currentFrame_ / endFrame_));

	currentFrame_++;

	// 残像アニメーション終了したら解放
	if (currentFrame_ >= endFrame_) {
		afterimage_.isActive = false;
	}
}