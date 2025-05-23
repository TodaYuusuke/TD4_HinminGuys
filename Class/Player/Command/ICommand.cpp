#include "ICommand.h"
#include "../Player.h"

MoveCommand::MoveCommand() {
	banInput_ = BanNone;
	currentInput_ = ~BanMove;
}

void MoveCommand::Exec(Player& player, int& banInput) {
	// 入力不可
	if (IsBitSame(banInput, BanMove, GetSetBitPosition(BanMove))) {
		player.GetSystemManager()->GetMoveSystem()->SetEnableInput(false);
		player.GetSystemManager()->GetMoveSystem()->SetIsActive(false);
		isActive_ = false;
		return;
	}

	// 入力時に必要な情報を設定
	player.GetSystemManager()->GetMoveSystem()->Command();
	// 入力禁止設定(移動時は入力禁止なし)
	banInput = banInput_;

	// 例外
	// 鞘投げしているときはパリィのみできない
	if (player.GetSystemManager()->GetSheathSystem()->GetIsActive()) {
		banInput |= BanParry;
	}
	// 鞘投げのクールタイム中は投げれない
	if (!player.GetSystemManager()->GetSheathSystem()->CheckCoolTime()) {
		banInput |= BanSheath;
	}
	// 鞘投げのクールタイム中は投げれない
	if (player.GetSystemManager()->GetAttackSystem()->GetIsEditingMode()) {
		banInput |= BanAttack;
	}


	isActive_ = true;
}

void MoveCommand::Reset(Player& player, int& banInput) {
	player;
	banInput;
}

NormalAttackCommand::NormalAttackCommand() {
	banInput_ = BanMove | BanParry | BanEvasion | BanSheath;
	currentInput_ = ~BanAttack;
}

void NormalAttackCommand::Exec(Player& player, int& banInput) {
	// パリィ中、回避中は攻撃できない
	if (IsBitSame(banInput, BanAttack, GetSetBitPosition(BanAttack))) { return; }

	player.GetSystemManager()->GetAttackSystem()->Command();
	banInput = banInput_;
	isActive_ = true;
}

void NormalAttackCommand::Reset(Player& player, int& banInput) {
	if (player.GetSystemManager()->GetAttackSystem()->GetIsThisRoot()) {
		if (player.GetSystemManager()->GetAttackSystem()->GetIsAttackRecovery()) {
			banInput = BanNone;
			isActive_ = false;
		}
	}
}

ParryCommand::ParryCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = ~BanParry;
}

void ParryCommand::Exec(Player& player, int& banInput) {
	if (IsBitSame(banInput, BanParry, GetSetBitPosition(BanParry))) { return; }

	player.GetSystemManager()->GetParrySystem()->Command();
	banInput = banInput_;
	isActive_ = true;
}

void ParryCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetParrySystem()->GetIsActive()) {
		banInput = BanNone;
		isActive_ = false;
	}
}

void LockOnCommand::Exec(Player& player, int& banInput) {
	player.GetSystemManager()->GetLockOnSystem()->Command();
}

void LockOnCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetLockOnSystem()->GetIsActive()) {
		banInput = BanNone;
		isActive_ = false;
	}
}

EvasionCommand::EvasionCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = ~BanEvasion;
}

void EvasionCommand::Exec(Player& player, int& banInput) {
	if (IsBitSame(banInput, BanEvasion, GetSetBitPosition(BanEvasion))) { return; }

	player.GetSystemManager()->GetEvasionSystem()->Command();
	banInput = banInput_;
	isActive_ = true;
}

void EvasionCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetEvasionSystem()->GetIsActive()) {
		banInput = BanNone;
		isActive_ = false;
	}
}

SheathCommand::SheathCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = ~BanSheath;
}

void SheathCommand::Exec(Player& player, int& banInput) {
	if (IsBitSame(banInput, BanSheath, GetSetBitPosition(BanSheath))) { return; }

	player.GetSystemManager()->GetSheathSystem()->Command();
	banInput = banInput_;
	isActive_ = true;
}

void SheathCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetSheathSystem()->GetSheathState()->GetIsActive()) {
		banInput = BanNone;
		isActive_ = false;
	}
}