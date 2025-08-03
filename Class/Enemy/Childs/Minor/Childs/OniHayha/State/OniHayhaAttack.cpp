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
	stateParameter_.idleParameter.countStandTime = stateParameter_.idleParameter.standTime + LWP::Utility::Random::GenerateFloat(0.0f, 0.5f);
	animation_.GetPlayBackSpeed() = 1.0f;
	bulletCollider_.isActive = false;
#ifdef _DEBUG
	capsule_.isShowWireFrame = false;
	tmpSphere_.isActive = false;
	tmpSphereSecond_.isActive = false;
#endif // _DEBUG
	capsule_.end = { 0.0f,0.0f,0.0f };
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
	//弾の方向を決める
	bulletDirection_ = GetPlayerPosition() - GetPosition();
	bulletDirection_.y = 0.0f;
	bulletDirection_ = bulletDirection_.Normalize();
	bulletCollider_.worldTF.translation = GetPosition();

	stateParameter_.attackParameter.currentFreezingTime = 0.0f;

	stateParameter_.attackParameter.isPlayedSE = false;

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

	//SEを鳴らす時間に到達したら鳴らす
	if (animation_.GetProgress() >= stateParameter_.attackParameter.sePlayTime and
		not stateParameter_.attackParameter.isPlayedSE) {
		sePlayer_->PlaySE("enemy/oniHayha/shot.mp3", "shot", 0.5f);
		stateParameter_.attackParameter.isPlayedSE = true;
	}

	//攻撃受付時間を超過したら判定オフ
	if (animation_.GetProgress() > stateParameter_.attackParameter.endAcceptTime) {
		bulletCollider_.isActive = false;
#ifdef _DEBUG
		tmpSphere_.isActive = false;
		tmpSphereSecond_.isActive = false;
#endif // _DEBUG
	}
	//開始と終了時間の間だけ判定を付ける
	else if (animation_.GetProgress() >= stateParameter_.attackParameter.startAcceptTime and
		animation_.GetProgress() <= stateParameter_.attackParameter.endAcceptTime) {
		bulletCollider_.isActive = true;

		capsule_.end = bulletDirection_ * stateParameter_.attackParameter.bulletSpeed;

		//弾を放ったら攻撃フラグをオフにする
		isAttack_ = false;
#ifdef _DEBUG
		tmpSphere_.worldTF.translation = bulletCollider_.worldTF.GetWorldPosition();
		tmpSphere_.isActive = true;
		tmpSphereSecond_.worldTF.translation = capsule_.end;
		tmpSphereSecond_.isActive = true;
#endif // _DEBUG

	}
	//開始時間未満も判定を付けない
	else {
		bulletCollider_.isActive = false;
#ifdef _DEBUG
		tmpSphere_.isActive = false;
		tmpSphereSecond_.isActive = false;
#endif // _DEBUG
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

