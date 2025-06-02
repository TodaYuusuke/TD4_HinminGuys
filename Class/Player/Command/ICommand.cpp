#include "ICommand.h"
#include "../Player.h"

MoveCommand::MoveCommand() {
	banInput_ = BanNone;
	currentInput_ = ~BanMove;
	isActive_ = false;
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
		//banInput |= BanParry;
		if (!IsBitSame(banInput, BanParry, GetSetBitPosition(BanParry))) {
			banInput |= BanParry;
		}
	}
	// 鞘投げのクールタイム中は投げれない
	if (!player.GetSystemManager()->GetSheathSystem()->CheckCoolTime()) {
		//banInput |= BanSheath;
		if (!IsBitSame(banInput, BanSheath, GetSetBitPosition(BanSheath))) {
			banInput |= BanSheath;
		}
	}
	// 攻撃の編集モード中は攻撃できない
	if (player.GetSystemManager()->GetAttackSystem()->GetIsEditingMode()) {
		if (!IsBitSame(banInput, BanAttack, GetSetBitPosition(BanAttack))) {
			banInput |= BanAttack;
		}
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
	isActive_ = false;
}

void NormalAttackCommand::Exec(Player& player, int& banInput) {
	// パリィ中、回避中は攻撃できない
	if (IsBitSame(banInput, BanAttack, GetSetBitPosition(BanAttack))) { return; }

	player.GetSystemManager()->GetAttackSystem()->Command();
	banInput = banInput_;
	isActive_ = true;
}

void NormalAttackCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetAttackSystem()->GetIsStiffness()) {
		banInput = BanNone;
		isActive_ = false;
	}
}

ParryCommand::ParryCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = ~BanParry;
	isActive_ = false;
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

LockOnCommand::LockOnCommand() {
	banInput_ = BanNone;
	currentInput_ = ~BanLockOn;
	isActive_ = false;
}

void LockOnCommand::Exec(Player& player, int& banInput) {
	// 入力不可
	if (IsBitSame(banInput, BanLockOn, GetSetBitPosition(BanLockOn))) { return; }

	player.GetSystemManager()->GetLockOnSystem()->Command();
}

void LockOnCommand::Reset(Player& player, int& banInput) {
	banInput = BanNone;
	isActive_ = false;
	player;
}

EvasionCommand::EvasionCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = ~BanEvasion;
	isActive_ = false;
}

void EvasionCommand::Exec(Player& player, int& banInput) {
	if (IsBitSame(banInput, BanEvasion, GetSetBitPosition(BanEvasion))) { return; }

	player.GetSystemManager()->GetEvasionSystem()->Command();
	banInput = banInput_;
	isActive_ = true;
}

void EvasionCommand::Reset(Player& player, int& banInput) {
	if (!player.GetSystemManager()->GetEvasionSystem()->GetIsActive()) {
		// ダメージを食らっていないとき
		if (!player.GetSystemManager()->GetDamageResponseSystem()->GetIsStun()) {
			banInput = BanNone;
			isActive_ = false;
		}
	}
}

SheathCommand::SheathCommand() {
	banInput_ = BanMove | BanParry | BanAttack | BanEvasion | BanSheath;
	currentInput_ = ~BanSheath;
	isActive_ = false;
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