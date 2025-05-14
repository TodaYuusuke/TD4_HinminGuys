#pragma once
#include "../IAttackSystemState.h"

class IEnemy;
class LockOnAttack : public IAttackSystemState {
public:
	// コンストラクタ
	LockOnAttack(Attack* attackSystem, Player* player, IEnemy* lockOnTarget);
	// デストラクタ
	~LockOnAttack() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:
	IEnemy* lockOnTarget_;
};