#include "NoneAttack.h"
#include "../../Attack.h"

NoneAttack::NoneAttack(Attack* attackSystem) {
	attackSystem_ = attackSystem;
	assistVel_ = { 0.0f,0.0f,0.0f };

	// AttackSystemクラスに送る
	attackSystem_->SetAttackAssistVel(assistVel_);
}

void NoneAttack::Initialize() {
	assistVel_ = { 0.0f,0.0f,0.0f };

	// AttackSystemクラスに送る
	attackSystem_->SetAttackAssistVel(assistVel_);
}

void NoneAttack::Update() {

}