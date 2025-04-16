#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// ボス敵停止状態クラス
/// </summary>
class BossEnemyIdle : public IEnemyState
{
public:

	~BossEnemyIdle() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:



};

