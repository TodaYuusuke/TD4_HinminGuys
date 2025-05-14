#pragma once
#include "../../IEnemyState.h"

using namespace LWP::Math;

/// <summary>
/// 雑魚敵移動状態クラス
/// </summary>
class NormalMove : public IEnemyState
{
public:

	~NormalMove() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

	static void DebugGUI();

private:

	//移動方向
	Vector3 velocity_;
	//移動時間
	static int32_t runTime_;
	//カウント
	int32_t countRunTime_;
	//攻撃モーションに入る距離
	static float attackDist_;
	//プレイヤーに近づくかどうか
	bool isApproach_ = true;

};

