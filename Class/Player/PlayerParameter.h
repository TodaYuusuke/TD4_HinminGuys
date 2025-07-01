#pragma once
#include "Adapter.h"

class Player;
class PlayerParameter {
public:
	// コンストラクタ
	PlayerParameter(Player* player);
	// デストラクタ
	~PlayerParameter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:
	/// <summary>
	/// 各パラメータの倍率の変更
	/// </summary>
	void CheckMultiply();

public:// Getter, Setter
#pragma region Getter
	float GetCurrentAttackStrength() { return currentAttackStrength_ = attackStrength_ * attackMultiply_; }
	float GetCurrentSheathDamageStrength() { return currentSheathDamageStrength_ = sheathDamegeStrength_; }
	float GetCurrentKnockBackStrength() { return currentKnockBackStrength_ = knockBackStrength_; }
	/// <summary>
	/// 鞘破壊状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsSheathBreak() { return isBreak_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	Player* player_;

public:// jsonで保存する値
	/// 鞘破壊時の倍率
	// 移動速度
	float sheathBreakMoveSpeedMultiply = 1.5f;
	// 攻撃力
	float sheathBreakAttackMultiply = 2.0f;
	// 防御
	float sheathBreakDefenseMultiply = 0.2f;

public:
	// 移動速度倍率
	float moveSpeedMultiply_ = 1.0f;
	// 攻撃力倍率
	float attackMultiply_ = 1.0f;
	// 防御倍率
	float defenseMultiply_ = 1.0f;

	// 攻撃力
	float attackStrength_;
	// 鞘に与えるダメージ
	float sheathDamegeStrength_;
	// 相手に与えるノックバックの強さ
	float knockBackStrength_;

private:
	LWP::Utility::JsonIO json_;

	// 現在の攻撃力
	float currentAttackStrength_;
	// 現在の鞘に与えるダメージ
	float currentSheathDamageStrength_;
	// 現在の相手に与えるノックバックの強さ
	float currentKnockBackStrength_;

	// 鞘破壊状態になったか
	bool isBreak_;
};