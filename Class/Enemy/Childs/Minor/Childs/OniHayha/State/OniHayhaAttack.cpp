#include "../../../../../../Player/Player.h"
#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"
#include "../../../../../../GameMask.h"

using namespace GameMask;
using namespace LWP::Math;
using namespace OniHayhaState;

void OniHayha::AttackFinalize([[maybe_unused]] const States& pre) {

	//待機状態に移行
	isAttack_ = false;
	isAttackPhase_ = false;
	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = stateParameter_.idleParameter.standTime;
	animation_.GetPlayBackSpeed() = 1.0f;

}

void OniHayha::AttackInit([[maybe_unused]] const States& pre)
{
	
	preState_ = States::kAttack;
	animation_.Play("Shot", 0.6f)
		.Loop(false);
	bulletCollider_.isActive = false;
	isAttack_ = true;
	//パリィエフェクトフラグリセット
	isActivationParryEffect_ = false;
	//弾のポジションセット
	sphere_.position = laserModel_.worldTF.GetWorldPosition();
	//弾の方向を決める
	bulletDirection_ = GetPlayerPosition() - GetPosition();
	bulletDirection_.y = 0.0f;
	bulletDirection_ = bulletDirection_.Normalize();

	stateParameter_.attackParameter.currentFreezingTime = 0.0f;

}

void OniHayha::AttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//パリィエフェクトが発生していないかつ、エフェクトの発生時間を超過したらパリィエフェクト発動
	if (not isActivationParryEffect_ and
		animation_.GetProgress() > stateParameter_.attackParameter.startAcceptTime - enemyManager_->GetParryEffectOccurTime()) {
		//パリィエフェクト開始
		StartParryEffect(model_.GetJointWorldPosition("Hips"));
		isActivationParryEffect_ = true;
	}

	//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
	if (isStartParryEffect_) {
		animation_.GetPlayBackSpeed() = 0.1f;
		bulletCollider_.isActive = false;
		laserModel_.isActive = true;
	}
	//パリィエフェクトが終わったら通常スピードで判定をオンにする
	else if(IsExitParryEffect()) {
		animation_.GetPlayBackSpeed() = 1.0f;
		laserModel_.isActive = false;
	}

	//攻撃受付時間を超過したら判定オフ
	if (animation_.GetProgress() > stateParameter_.attackParameter.endAcceptTime and bulletCollider_.isActive) {
		bulletCollider_.isActive = false;
	}
	//開始と終了時間の間だけ判定を付ける
	else if (animation_.GetProgress() >= stateParameter_.attackParameter.startAcceptTime and
		animation_.GetProgress() <= stateParameter_.attackParameter.endAcceptTime) {
		bulletCollider_.isActive = true;

		//弾を放ったら攻撃フラグをオフにする
		isAttack_ = false;
		//攻撃受付中は決めたベクトルに向かって移動
		sphere_.position += bulletDirection_ * stateParameter_.attackParameter.bulletSpeed * LWP::Info::GetDeltaTimeF();


	}
	//開始時間未満も判定を付けない
	else {
		bulletCollider_.isActive = false;
	}


	//攻撃が終了した時
	if (not animation_.GetPlaying()) {

		//硬直時間をカウント
		if (stateParameter_.attackParameter.currentFreezingTime < stateParameter_.attackParameter.freezingTime) {

			stateParameter_.attackParameter.currentFreezingTime += 1.0f * LWP::Info::GetDeltaTimeF();

			//カウント超えたら終わり
			if (stateParameter_.attackParameter.currentFreezingTime >= stateParameter_.attackParameter.freezingTime) {
				isAttack_ = false;
				state_.request = States::kIdle;
				return;
			}

		}

	}

	SetPosition(GetPosition() + (GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));

}

