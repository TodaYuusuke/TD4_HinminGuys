#include "HitReaction.h"
#include "../../../../Player/Player.h"
#include "../Normal.h"
#include "../../../EnemyManager.h"
#include "States.h"

float HitReaction::decay_ = 0.9f;

HitReaction::HitReaction(Normal* enemy)
{

	enemy_ = enemy;
	enemy_->SetAnimation("Damage", false);
	stateType_ = States::kHitReaction;

}

void HitReaction::Initialize()
{
	
}

void HitReaction::Update()
{

	enemy_->SetPosition(enemy_->GetPosition() + (enemy_->GetKnockBackVelocity() + enemy_->GetRepulsiveForce()) *
		LWP::Info::GetDeltaTime());
	//ノックバック減衰
	enemy_->SetKnockBackVelocity(enemy_->GetKnockBackVelocity() * decay_);

	//攻撃が終了した時
	if (not enemy_->GetAnimation()->GetPlaying()) {

		//攻撃判定オフ
		enemy_->GetSwordCollider().isActive = false;
		//待機状態に移行
		enemy_->EndAttack();
		enemy_->SetIsAttackPhase(false);
		//直前の状態を再開
		enemy_->SetState(enemy_->GetPreState(), false);
		return;

	}

}

void HitReaction::DebugGUI()
{

	

}
