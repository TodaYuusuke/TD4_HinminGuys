#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// ボス敵停止状態クラス
/// </summary>
class BossIdle : public IEnemyState
{
public:

	~BossIdle() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

	static void DebugGUI();

private:



};

