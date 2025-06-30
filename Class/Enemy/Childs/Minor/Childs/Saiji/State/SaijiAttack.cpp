#include "../../../../../../Player/Player.h"
#include "../Saiji.h"
#include "../../../../../EnemyManager.h"
#include "../../../../../../GameMask.h"

using namespace GameMask;
using namespace LWP::Math;
using namespace SaijiState;

void Saiji::AttackFinalize([[maybe_unused]] const States& pre) {

	//待機状態に移行
	isAttack_ = false;
	isAttackPhase_ = false;
	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = stateParameter_.idleParameter.standTime;

}

void Saiji::AttackInit([[maybe_unused]] const States& pre)
{
	
	preState_ = States::kAttack;
	SetAnimation("LightAttack2", false, 0.1f);
	swordCollider_.isActive = false;
	isAttack_ = true;
	//パリィエフェクト開始
	StartParryEffect(swordModel_.GetJointWorldPosition("Grip"));

}

void Saiji::AttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
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
	if (animation_.GetProgress() > stateParameter_.attackParameter.endAcceptTime and swordCollider_.isActive) {
		isAttack_ = false;
		swordCollider_.isActive = false;
	}
	//開始と終了時間の間だけ判定を付ける
	else if (animation_.GetProgress() >= stateParameter_.attackParameter.startAcceptTime and
		animation_.GetProgress() <= stateParameter_.attackParameter.endAcceptTime) {
		swordCollider_.isActive = true;
	}
	//開始時間未満も判定を付けない
	else {
		swordCollider_.isActive = false;
	}

	//攻撃が終了した時
	if (not animation_.GetPlaying()) {
		isAttack_ = false;
		state_.request = States::kIdle;
		return;

	}

	SetPosition(GetPosition() + (GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));

}

