#pragma once
#include "../INormalEnemyState.h"

using namespace LWP::Math;

class Following : public INormalEnemyState
{
public:
	Following(Normal* enemy);
	~Following() override;

	void Initialize() override;

	void Update() override;

	static void DebugGUI();

private:

	//待機モーションに移行する距離
	static float idleDist_;

};


