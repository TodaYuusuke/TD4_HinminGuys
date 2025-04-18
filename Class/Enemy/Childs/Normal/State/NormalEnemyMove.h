#pragma once
#include "../../../IEnemyState.h"

using namespace LWP::Math;

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

	//移動方向
	Vector3 velocity_;
	//移動時間
	int32_t runTime_;
	//追加移動時間
	int32_t overRunTime_;
	//攻撃モーションに入る距離
	float attackDist_ = 0.5f;
	//近づくかどうかを決める距離
	float approachDist_ = 1.5f;
	//プレイヤーに近づくかどうか
	bool isApproach_ = true;

};

