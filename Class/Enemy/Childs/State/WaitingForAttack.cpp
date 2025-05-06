#include "WaitingForAttack.h"
#include "NormalAttack.h"
#include "../../IEnemy.h"

uint16_t WaitingForAttack::attackCount_ = 0;
uint16_t WaitingForAttack::nextAttackCount_ = 0;

WaitingForAttack::WaitingForAttack()
{
	//現在の攻撃カウントから順番を決める
	attackID_ = attackCount_;
	//攻撃の順番を決める数字を上昇させる
	attackCount_++;

}

WaitingForAttack::~WaitingForAttack()
{

	//次攻撃する番号を上昇させる
	nextAttackCount_++;

}

void WaitingForAttack::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Idle", true);
}

void WaitingForAttack::Update()
{

	//誰も攻撃しておらず、順番が回ってきたら攻撃に移行
	if (not enemy_->GetIsAttack() and attackID_ == nextAttackCount_) {
		//攻撃状態に移行
		enemy_->SetState(new NormalAttack());
		return;
	}

}
