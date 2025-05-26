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
	
}

void AttackRecovery::AnimCommand() {
}
