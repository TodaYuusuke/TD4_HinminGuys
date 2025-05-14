#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// ボス敵攻撃状態クラス
/// </summary>
class BossAttack : public IEnemyState
{
public:

	~BossAttack() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

	static void DebugGUI();

private:



};

