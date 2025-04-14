#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// ボス敵移動状態クラス
/// </summary>
class BossEnemyMove : public IEnemyState
{
public:

	~BossEnemyMove() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:



};

