#pragma once
#include "../INormalEnemyState.h"

/// <summary>
/// 雑魚敵攻撃状態クラス
/// </summary>
class NormalAttack : public INormalEnemyState
{
public:

	NormalAttack(Normal* enemy);

	~NormalAttack() override = default;

	void Initialize() override;

	void Update() override;

	static void DebugGUI();

private:

	static float attackAcceptTime_;

};

