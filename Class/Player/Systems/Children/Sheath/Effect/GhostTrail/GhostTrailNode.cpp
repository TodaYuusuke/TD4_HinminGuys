#include "GhostTrailNode.h"
#include "../../../../../../Components/HitStopController.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

GhostTrailNode::GhostTrailNode(const std::string& fileName, const std::string& materialName, const float& maxElapseFrame, LWP::Object::TransformQuat* target) {
	ghost_.LoadShortPath(fileName);
	materialName_ = materialName;
	maxElapseFrame_ = maxElapseFrame;
	target_ = target;
	// 薄暗くする
	ghost_.materials[materialName_].color = {
		100,
		100,
		100,
		122
	};
}

void GhostTrailNode::Initialize() {
	velocity_ = { 0.0f,0.0f,0.0f };
	isAlive_ = true;
	// SRT適用
	ghost_.worldTF.scale = target_->scale;
	ghost_.worldTF.rotation = target_->rotation;
	ghost_.worldTF.translation = target_->GetWorldPosition();

}

void GhostTrailNode::Update() {
	if (!isAlive_) { return; }

	// 速度
	ghost_.worldTF.translation += velocity_;

	// 透明にする
	ghost_.materials[materialName_].color.A = static_cast<int>(LerpF(122, 0.0f, currentFrame_ / maxElapseFrame_));

	// 生存時間を過ぎている
	if (currentFrame_ >= maxElapseFrame_) {
		isAlive_ = false;
	}

	currentFrame_ += HitStopController::GetInstance()->GetDeltaTime();
}