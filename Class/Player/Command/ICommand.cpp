#include "ICommand.h"
#include "../Player.h"

void NormalAttackCommand::Exec(Player& player) {
	player.GetAttackSystem()->NormalCommand();
}

void ParryCommand::Exec(Player& player) {
	player.GetParrySystem()->Command();
}

void LockOnCommand::Exec(Player& player) {
	player.GetLockOnSystem()->Command();
}