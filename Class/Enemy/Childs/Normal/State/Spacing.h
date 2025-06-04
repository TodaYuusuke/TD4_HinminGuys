#pragma once
#include "../INormalEnemyState.h"

using namespace LWP::Math;

/// <summary>
/// 間合いを上手く取るクラス
/// </summary>
class Spacing : public INormalEnemyState
{
public:
	Spacing(Normal* enemy);
	~Spacing() override;

	void Initialize() override;

	void Update() override;

	static void DebugGUI();

private:

	//間合いを取る時間
	static int32_t spacingTime_;
	//間合い距離
	static float spaceDist_;

};


