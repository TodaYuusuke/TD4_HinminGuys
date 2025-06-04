#include "AttackRecovery.h"
#include "Idle.h"
#include "../../../../Player.h"

AttackRecovery::AttackRecovery(Move* moveSystem, Player* player) {
	moveSystem_ = moveSystem;
	player_ = player;

	// 状態の名前を付与
	stateName_ = "AttackRecovery";

	AnimCommand();
}

void AttackRecovery::Initialize() {

}

void AttackRecovery::Update() {
	if (!player_->GetAnimation()->GetPlaying()) {
		moveSystem_->ChangeState(new Idle(moveSystem_, player_));
		return;
	}
}

void AttackRecovery::AnimCommand() {
}
