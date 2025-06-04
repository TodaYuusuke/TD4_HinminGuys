#pragma once
#include "../INormalEnemyState.h"

/// <summary>
/// 雑魚敵停止状態クラス
/// </summary>
class NormalIdle : public INormalEnemyState
{
public:
	
	NormalIdle(Normal* enemy);

	~NormalIdle() override = default;

	void Initialize() override;

	void Update() override;

	static void DebugGUI();

private:

	//待機時間
	static int32_t standTime_;
	//追従モーションに移行する距離
	static float followingDist_;

};

