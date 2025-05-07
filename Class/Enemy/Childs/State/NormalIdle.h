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

	static void DebugGUI();

private:

	//待機時間
	static int32_t standTime_;
	//カウント
	int32_t countStandTime_;
	//追従モーションに移行する距離
	static float followingDist_;

};

