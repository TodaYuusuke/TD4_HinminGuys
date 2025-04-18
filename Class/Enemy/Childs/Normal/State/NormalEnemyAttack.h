#pragma once
#include "../../../IEnemyState.h"

/// <summary>
/// 雑魚敵攻撃状態クラス
/// </summary>
class NormalEnemyAttack : public IEnemyState
{
public:

	~NormalEnemyAttack() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:



};

