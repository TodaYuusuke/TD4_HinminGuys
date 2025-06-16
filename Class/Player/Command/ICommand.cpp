#include "ICommand.h"
#include "../Player.h"

void MoveCommand::Exec(Player& player) {
	player;
}

void NormalAttackCommand::Exec(Player& player) {
	player;
}

void ParryCommand::Exec(Player& player) {
	player;
}

void LockOnCommand::Exec(Player& player) {
	player.GetSystemManager()->GetLockOnSystem()->Command();
}

void EvasionCommand::Exec(Player& player) {
	player;
}

void SheathCommand::Exec(Player& player) {
	player;
}