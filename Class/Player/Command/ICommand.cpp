#include "ICommand.h"
#include "../Player.h"

MoveCommand::MoveCommand() {
	banInput_ = BanNone;
	currentInput_ = BanMove;
}

void MoveCommand::Exec(Player& player, std::vector<ICommand*>* pCommand, int& banInput) {
	//pCommand;
	//if (IsBitSame(banInput, BanMove, GetSetBitPosition(BanMove))) { 
	//	player.GetSystemManager()->GetMoveSystem()->SetEnableInput(false);
	//	player.GetSystemManager()->GetMoveSystem()->SetIsActive(false);
	//	isActive_ = false;
	//	return; 
	//}

	//player.GetSystemManager()->GetMoveSystem()->Command();

	//// 移動機能のみ動いているときは入力不可状態をなくす
	//if (!player.GetSystemManager()->GetAttackSystem()->GetIsActive() && !player.GetSystemManager()->GetEvasionSystem()->GetIsActive()
	//	&& !player.GetSystemManager()->GetParrySystem()->GetIsActive() && !player.GetSystemManager()->GetSheathSystem()->GetIsActive()) {
	//	banInput = banInput_;
	//}

	if (player.GetSystemManager()->GetNextEnableState() == InputMove) {

	}

	isActive_ = true;
}

void MoveCommand::Reset(Player& player, int& banInput) {
	player;
	banInput;
	isActive_ = false;
}

NormalAttackCommand::NormalAttackCommand() {

}

void NormalAttackCommand::Exec(Player& player, std::vector<ICommand*>* pCommand, int& banInput) {
	// パリィ中、回避中は攻撃できない
	//if (player.GetSystemManager()->GetParrySystem()->GetIsActive() || player.GetSystemManager()->GetEvasionSystem()->GetIsActive()) { return; }

	player.GetSystemManager()->GetAttackSystem()->NormalCommand();
	banInput;
	pCommand;
}

void NormalAttackCommand::Reset(Player& player, int& banInput) {
	player;
	banInput;
}

ParryCommand::ParryCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = BanParry;
}

void ParryCommand::Exec(Player& player, std::vector<ICommand*>* pCommand, int& banInput) {
	if (IsBitSame(banInput, BanParry, GetSetBitPosition(BanParry))) { return; }

	player.GetSystemManager()->GetParrySystem()->Command();
//	banInput = banInput_;
	(*pCommand).push_back(this);
	isActive_ = true;
}

void ParryCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetParrySystem()->GetIsActive()) {
		//banInput = BanNone;
		isActive_ = false;
	}
}

void LockOnCommand::Exec(Player& player, std::vector<ICommand*>* pCommand, int& banInput) {
	player.GetSystemManager()->GetLockOnSystem()->Command();
	banInput;
}

void LockOnCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetLockOnSystem()->GetIsActive()) {
		//banInput = BanNone;
		isActive_ = false;
	}
}

EvasionCommand::EvasionCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = BanEvasion;
}

void EvasionCommand::Exec(Player& player, std::vector<ICommand*>* pCommand, int& banInput) {
	if (IsBitSame(banInput, BanEvasion, GetSetBitPosition(BanEvasion))) { return; }

	player.GetSystemManager()->GetEvasionSystem()->Command();
	//banInput = banInput_;
	(*pCommand).push_back(this);
	isActive_ = true;
}

void EvasionCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetEvasionSystem()->GetIsActive()) {
		//banInput = BanNone;
		isActive_ = false;
	}
}

SheathCommand::SheathCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = BanSheath;
}

void SheathCommand::Exec(Player& player, std::vector<ICommand*>* pCommand, int& banInput) {
	if (IsBitSame(banInput, BanSheath, GetSetBitPosition(BanSheath))) { return; }

	player.GetSystemManager()->GetSheathSystem()->Command();

	(*pCommand).push_back(this);
	isActive_ = true;
}

void SheathCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetSheathSystem()->GetIsActive()) {
		//banInput = BanNone;
		isActive_ = false;
	}
}