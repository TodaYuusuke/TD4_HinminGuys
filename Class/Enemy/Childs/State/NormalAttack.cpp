#include "NormalAttack.h"
#include "NormalIdle.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void NormalAttack::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Attack", false);
	//攻撃判定オン
	enemy_->GetAttackHitBox()->isActive = true;
	enemy_->GetAttackHitBox()->isShowWireFrame = true;
}

void NormalAttack::Update()
{

	//攻撃が終了した時
	if (not enemy_->GetAnimation()->GetPlaying()) {

		//攻撃判定オフ
		enemy_->GetAttackHitBox()->isActive = false;
		enemy_->GetAttackHitBox()->isShowWireFrame = false;

		//待機状態に移行
		enemy_->SetState(new NormalIdle());

	}

}
