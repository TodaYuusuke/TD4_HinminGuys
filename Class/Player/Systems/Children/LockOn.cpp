#include "LockOn.h"

LockOn::LockOn(LWP::Object::Camera* camera) {
	pCamera_ = camera;
}

void LockOn::Initialize() {
	isActive_ = false;
}

void LockOn::Update() {
	//if(isActive_){}
}

void LockOn::Reset() {

}

void LockOn::InputUpdate() {
	// LTでロックオン
	if (LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_LEFT_THUMB)) {
		if (!isActive_) {
			isActive_ = true;
		}
		else {
			isActive_ = false;
		}
	}
}