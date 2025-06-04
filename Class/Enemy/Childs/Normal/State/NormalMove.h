#pragma once
#include "../INormalEnemyState.h"

using namespace LWP::Math;

/// <summary>
/// 雑魚敵移動状態クラス
/// </summary>
class NormalMove : public INormalEnemyState
{
public:

	NormalMove(Normal* enemy);

	~NormalMove() override = default;

	void Initialize() override;

	void Update() override;

	static void DebugGUI();

private:

	//移動時間
	static int32_t runTime_;
	//攻撃モーションに入る距離
	static float attackDist_;

};

