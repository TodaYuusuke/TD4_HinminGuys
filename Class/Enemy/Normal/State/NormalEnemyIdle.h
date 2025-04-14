#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// 雑魚敵停止状態クラス
/// </summary>
class NormalEnemyIdle : public IEnemyState
{
public:
	
	~NormalEnemyIdle() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:



};

