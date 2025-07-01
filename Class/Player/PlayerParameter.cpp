#include "PlayerParameter.h"
#include "Player.h"

PlayerParameter::PlayerParameter(Player* player) {
	player_ = player;

	json_.Init("PlayerParameter.json");
	json_.BeginGroup("SheathBreakMultiply")
	.AddValue("Attack", &sheathBreakAttackMultiply)
	.AddValue("MoveSpeed", &sheathBreakMoveSpeedMultiply)
	.AddValue("Defense", &sheathBreakDefenseMultiply)
	.EndGroup()
	.CheckJsonFile();
}

void PlayerParameter::Initialize() {

}

void PlayerParameter::Update() {
	// 各パラメータの倍率の変更
	CheckMultiply();

	// 現在の攻撃力
	currentAttackStrength_ = attackStrength_ * attackMultiply_;
	// 現在の鞘のダメージ量
	currentSheathDamageStrength_ = sheathDamegeStrength_;
	// 現在のノックバック量
	currentKnockBackStrength_ = knockBackStrength_;
}

void PlayerParameter::DebugGui() {
	json_.DebugGUI();
}

void PlayerParameter::CheckMultiply() {
	// 鞘破壊状態
	if (player_->GetUIManager()->GetSheathGauge().GetIsIncrease()) {
		isBreak_ = true;
		// 移動速度
		moveSpeedMultiply_ = sheathBreakMoveSpeedMultiply;
		// 攻撃力
		attackMultiply_ = sheathBreakAttackMultiply;
		// 防御
		defenseMultiply_ = sheathBreakDefenseMultiply;
	}
	else {
		isBreak_ = false;
		// 移動速度
		moveSpeedMultiply_ = 1.0f;
		// 攻撃力
		attackMultiply_ = 1.0f;
		// 防御
		defenseMultiply_ = 1.0f;
	}
}