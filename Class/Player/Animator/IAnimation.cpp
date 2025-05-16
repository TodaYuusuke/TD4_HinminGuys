#include "IAnimation.h"
#include "../Player.h"

void MoveAnim::Exec(Player& player) {
	player.GetSystemManager()->GetMoveSystem()->AnimCommand();
}

void AttackAnim::Exec(Player& player) {
	//player.GetSystemManager()->GetAttackSystem()->AnimCommand();
}

void ParryAnim::Exec(Player& player) {
	if (!player.GetSystemManager()->GetParrySystem()->GetTrigger()) { return; }

	player.GetSystemManager()->GetParrySystem()->AnimCommand();
}

void EvasionAnim::Exec(Player& player) {
	if (!player.GetSystemManager()->GetEvasionSystem()->GetTrigger()) { return; }

	player.GetSystemManager()->GetEvasionSystem()->AnimCommand();
}

void SheathAnim::Exec(Player& player) {
	if (!player.GetSystemManager()->GetSheathSystem()->GetTrigger()) { return; }

	player.GetSystemManager()->GetSheathSystem()->AnimCommand();
}
