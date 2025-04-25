#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// 雑魚敵停止状態クラス
/// </summary>
class NormalIdle : public IEnemyState
{
public:
	
	~NormalIdle() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:

	//待機時間
	int32_t standTime_;
	//攻撃モーションに入る距離
	float attackDist_ = 3.0f;


};

