#pragma once
#include "../Adapter/Adapter.h"


/// <summary>
/// プレイヤーと敵共通のルールがある変数などをまとめたもの
/// </summary>
class World
{
public:
	
	World();
	~World() = default;

	void DebugGUI();

	float GetWorldLimitRadius() const { return worldLimitRadius_; }

private:

	LWP::Utility::JsonIO json_;

	//世界の制限範囲
	float worldLimitRadius_ = 40.0f;

};
