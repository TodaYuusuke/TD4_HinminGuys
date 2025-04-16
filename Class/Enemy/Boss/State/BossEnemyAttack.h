#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// ボス敵攻撃状態クラス
/// </summary>
class BossEnemyAttack : public IEnemyState
{
public:

	~BossEnemyAttack() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:



};

