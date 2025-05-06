#pragma once
#include "../../IEnemyState.h"

using namespace LWP::Math;

/// <summary>
/// 他の敵が攻撃している間、待機するクラス
/// </summary>
class WaitingForAttack : public IEnemyState
{
public:
	WaitingForAttack();
	~WaitingForAttack() override;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

	static uint16_t GetAttackCount() { return attackCount_; }
	static uint16_t GetNextAttackCount() { return nextAttackCount_; }

private:

	//攻撃の順番を決める変数
	static uint16_t attackCount_;
	//次攻撃するのは何番かを決める変数
	static uint16_t nextAttackCount_;
	//自身の攻撃する順番
	uint16_t attackID_;
	//右回りかどうか
	bool isClockwise_ = false;

};
