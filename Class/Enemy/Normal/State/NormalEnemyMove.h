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

	//待機時間
	int32_t runTime_;
	//攻撃モーションに入る距離
	float attackDist_ = 3.0f;

};

