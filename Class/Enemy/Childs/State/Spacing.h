#pragma once
#include "../../IEnemyState.h"

using namespace LWP::Math;

/// <summary>
/// 間合いを上手く取るクラス
/// </summary>
class Spacing : public IEnemyState
{
public:
	Spacing();
	~Spacing() override;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:

	//近づくかどうかを決める距離
	float approachDist_ = 2.0f;
	//プレイヤーに近づくかどうか
	bool isApproach_ = true;

};


