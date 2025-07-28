#include "GhostTrail.h"
#include "../../../../../../Components/HitStopController.h"

GhostTrail::GhostTrail(const std::string& fileName, std::string materialName, LWP::Object::TransformQuat* target) {
	modelFileName_ = fileName;
	materialName_ = materialName;
	target_ = target;
}

GhostTrail::~GhostTrail() {
	for (GhostTrailNode* ghost : ghostNodes_) {
		delete ghost;
	}
	ghostNodes_.clear();
}

void GhostTrail::Initialize() {

}

void GhostTrail::Update() {
	if (!isActive_) { return; }

	for (auto it = ghostNodes_.begin(); it != ghostNodes_.end(); ) {
		GhostTrailNode* ghost = *it;
		if (!ghost->GetIsAlive()) {
			delete ghost;
			it = ghostNodes_.erase(it); // eraseの戻り値で次に進む
		}
		else {
			ghost->Update();
			++it;
		}
	}

	if (ghostIntervalFrame <= currentFrame_) {
		// 経過時間初期化
		currentFrame_ = 0.0f;
		ghostNodes_.push_back(CreateGhostTrailNode());
	}

	currentFrame_ += HitStopController::GetInstance()->GetDeltaTime();
}

GhostTrailNode* GhostTrail::CreateGhostTrailNode() {
	GhostTrailNode* ghostNode = new GhostTrailNode(modelFileName_, materialName_, maxElapseFrame, target_);
	ghostNode->Initialize();
	return ghostNode;
}
