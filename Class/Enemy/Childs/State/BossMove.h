#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// ボス敵移動状態クラス
/// </summary>
class BossMove : public IEnemyState
{
public:

	~BossMove() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:



};

