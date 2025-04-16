#include "AttackSystem.h"

void AttackSystem::Initialize() {

}

void AttackSystem::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }
}

void AttackSystem::Reset() {
	isActive_ = true;
}