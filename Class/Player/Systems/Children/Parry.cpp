#include "Parry.h"

Parry::Parry(LWP::Object::Camera* camera) {
	pCamera_ = camera;
}

void Parry::Initialize() {

}

void Parry::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }
}

void Parry::Reset() {

}
