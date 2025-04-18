#include "Attack.h"

Attack::Attack(LWP::Object::Camera* camera) {
	pCamera_ = camera;
}

void Attack::Initialize() {

}

void Attack::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }
}

void Attack::Reset() {
	isActive_ = true;
}