#include "AttackRecovery.h"
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
	// コンボの受付状態が終了したらIdle状態に移行
	if (!player_->GetSystemManager()->GetAttackSystem()->GetIsRecept()) {
		player_->GetSystemManager()->GetAttackSystem()->SetIsAttackRecovery(false);
	}
}

void AttackRecovery::AnimCommand() {
	// コンボの受付状態が終了したらIdle状態に移行
	if (!player_->GetSystemManager()->GetAttackSystem()->GetIsRecept()) {
		player_->GetSystemManager()->GetAttackSystem()->SetIsAttackRecovery(false);
	}
}
