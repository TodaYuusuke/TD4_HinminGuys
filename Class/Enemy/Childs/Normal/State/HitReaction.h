#pragma once
#include "../INormalEnemyState.h"

/// <summary>
/// 雑魚敵被弾リアクションクラス
/// </summary>
class HitReaction : public INormalEnemyState
{
public:

	HitReaction(Normal* enemy);

	~HitReaction() override = default;

	void Initialize() override;

	void Update() override;

	static void DebugGUI();

private:

	static float decay_;

};

