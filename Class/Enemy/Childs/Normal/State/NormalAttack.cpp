#include "../../../../Player/Player.h"
#include "../Normal.h"
#include "../../../EnemyManager.h"
#include "../../../../GameMask.h"

using namespace GameMask;
using namespace LWP::Math;

void Normal::AttackFinalize(const States& pre) {

	//待機状態に移行
	EndAttack();
	SetIsAttackPhase(false);
	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = IdleParameter::standTime_;

}

void Normal::AttackInit(const States& pre)
{
	
	preState_ = States::kNormalAttack;
	SetAnimation("LightAttack2", false, 0.1f);
	swordCollider_.isActive = false;
	BeginAttack();
	//パリィエフェクト開始
	StartParryEffect();

}

void Normal::AttackUpdate(std::optional<States>& req, const States& pre)
{

	//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
	if (isStartParryEffect_) {
		animation_.GetPlayBackSpeed() = 0.1f;
		swordCollider_.isActive = false;
	}
	//パリィエフェクトが終わったら通常スピードで判定をオンにする
	else if(IsExitParryEffect()) {
		animation_.GetPlayBackSpeed() = 1.0f;
		swordCollider_.isActive = true;
	}

	//攻撃受付時間を超過したら判定オフ
	if (animation_.GetProgress() > AttackParameter::attackAcceptTime_ and swordCollider_.isActive) {
		EndAttack();
		swordCollider_.isActive = false;
	}

	//攻撃が終了した時
	if (not animation_.GetPlaying()) {
		state_.request = States::kNormalIdle;
		return;

	}

	SetPosition(GetPosition() + (GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

}

