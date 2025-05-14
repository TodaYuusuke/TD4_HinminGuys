#include "ICommand.h"
#include "../Player.h"

void NormalAttackCommand::Exec(Player& player) {
	// パリィ中、回避中は攻撃できない
	if (player.GetSystemManager()->GetParrySystem()->GetIsActive() || player.GetSystemManager()->GetEvasionSystem()->GetIsActive()) { return; }

	player.GetSystemManager()->GetAttackSystem()->NormalCommand();
}

void ParryCommand::Exec(Player& player) {
	// 攻撃中、回避中はパリィできない
	if (player.GetSystemManager()->GetAttackSystem()->GetIsActive() || player.GetSystemManager()->GetEvasionSystem()->GetIsActive()) { return; }

	player.GetSystemManager()->GetParrySystem()->Command();
}

void LockOnCommand::Exec(Player& player) {
	player.GetSystemManager()->GetLockOnSystem()->Command();
}

void EvasionCommand::Exec(Player& player) {
	// パリィ中と攻撃中は回避できない
	if (player.GetSystemManager()->GetParrySystem()->GetIsActive() || player.GetSystemManager()->GetAttackSystem()->GetIsActive()) { return; }

	player.GetSystemManager()->GetEvasionSystem()->Command();
}

void SheathCommand::Exec(Player& player) {
	// パリィ中、回避中、攻撃中は鞘関連の行動ができない
	if (player.GetSystemManager()->GetParrySystem()->GetIsActive() || player.GetSystemManager()->GetEvasionSystem()->GetIsActive() || player.GetSystemManager()->GetAttackSystem()->GetIsActive()) { return; }

	player.GetSystemManager()->GetSheathSystem()->Command();
}