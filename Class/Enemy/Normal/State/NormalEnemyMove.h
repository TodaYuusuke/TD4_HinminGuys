#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// 雑魚敵移動状態クラス
/// </summary>
class NormalEnemyMove : public IEnemyState
{
public:

	~NormalEnemyMove() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:



};

