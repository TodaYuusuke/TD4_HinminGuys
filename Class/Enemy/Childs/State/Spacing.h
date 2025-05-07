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

	static void DebugGUI();

private:

	//間合いを取る時間
	static int32_t spacingTime_;
	//カウント
	int32_t countSpacingTime_;
	//間合い距離
	static float spaceDist_;
	//右回りかどうか
	bool isClockwise_ = false;

};


