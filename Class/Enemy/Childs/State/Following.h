#pragma once
#include "../../IEnemyState.h"

using namespace LWP::Math;

class Following : public IEnemyState
{
public:
	Following();
	~Following() override;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

	static void DebugGUI();

private:

	//待機モーションに移行する距離
	static float idleDist_;

};


