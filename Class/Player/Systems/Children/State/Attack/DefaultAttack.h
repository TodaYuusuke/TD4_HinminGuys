#pragma once
#include "../IAttackSystemState.h"

class DefaultAttack : public IAttackSystemState {
public:
	// コンストラクタ
	DefaultAttack(Attack* attackSystem, Player* player);
	// デストラクタ
	~DefaultAttack() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

};