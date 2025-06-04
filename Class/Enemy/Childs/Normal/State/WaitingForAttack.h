#pragma once
#include "../INormalEnemyState.h"

using namespace LWP::Math;

/// <summary>
/// 他の敵が攻撃している間、待機するクラス
/// </summary>
class WaitingForAttack : public INormalEnemyState
{
public:
	WaitingForAttack(Normal* enemy);
	~WaitingForAttack() override;

	void Initialize() override;

	void Update() override;

	static void DebugGUI();

	static uint16_t GetAttackCount() { return attackCount_; }
	static uint16_t GetNextAttackCount() { return nextAttackCount_; }

private:

	//攻撃の順番を決める変数
	static uint16_t attackCount_;
	//次攻撃するのは何番かを決める変数
	static uint16_t nextAttackCount_;

};
