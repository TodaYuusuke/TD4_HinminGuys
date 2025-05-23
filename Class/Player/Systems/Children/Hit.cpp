#include "Hit.h"

void Hit::Initialize() {
	hp_.Initialize();
}

void Hit::Update() {
	hp_.Update();
}

void Hit::Reset() {

}

void Hit::DebugGUI() {
	hp_.DebugGUI();
}

void Hit::CreateJsonFIle() {

}

void Hit::HitUpdate() {
	if (currentFrame_ <= 0) {
		currentFrame_ = endFrame_;
		isHit_ = false;
	}
}
