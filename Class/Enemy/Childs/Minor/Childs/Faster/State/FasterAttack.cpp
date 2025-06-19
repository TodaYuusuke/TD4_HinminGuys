#include "../../../../../../Player/Player.h"
#include "../Faster.h"
#include "../../../../../EnemyManager.h"
#include "../../../../../../GameMask.h"

using namespace GameMask;
using namespace LWP::Math;
using namespace FasterState;

void Faster::AttackFinalize(const States& pre) {

	//待機状態に移行
	EndAttack();
	SetIsAttackPhase(false);
	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = IdleParameter::standTime;

}

void Faster::AttackInit(const States& pre)
{
	
	preState_ = States::kAttack;
	SetAnimation("LightAttack2", false, 0.1f);
	swordCollider_.isActive = false;
	BeginAttack();
	//パリィエフェクト開始
	StartParryEffect();

}

void Faster::AttackUpdate(std::optional<States>& req, const States& pre)
{

	//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
	if (isStartParryEffect_) {
		animation_.GetPlayBackSpeed() = 0.1f;
		swordCollider_.isActive = false;
	}
	//パリィエフェクトが終わったら通常スピードで判定をオンにする
	else if(IsExitParryEffect()) {
		animation_.GetPlayBackSpeed() = 1.0f;
	}

	//攻撃受付時間を超過したら判定オフ
	if (animation_.GetProgress() > AttackParameter::endAcceptTime and swordCollider_.isActive) {
		EndAttack();
		swordCollider_.isActive = false;
	}
	//開始と終了時間の間だけ判定を付ける
	else if (animation_.GetProgress() >= AttackParameter::startAcceptTime and
		animation_.GetProgress() <= AttackParameter::endAcceptTime) {
		swordCollider_.isActive = true;
	}
	//開始時間未満も判定を付けない
	else {
		swordCollider_.isActive = false;
	}

	//攻撃が終了した時
	if (not animation_.GetPlaying()) {
		EndAttack();
		state_.request = States::kIdle;
		return;

	}

	SetPosition(GetPosition() + (GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

}

