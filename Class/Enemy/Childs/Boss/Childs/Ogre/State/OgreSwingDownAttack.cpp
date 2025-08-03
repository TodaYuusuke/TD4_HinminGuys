#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// ふり降ろし攻撃の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::SwingDownAttackFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	isAttack_ = false;
	isAttackPhase_ = false;
	//弱攻撃終了時の抽選処理
	EndLightAttack();
	aabbAttackCollider_.isActive = false;
#ifdef _DEBUG
	aabbAttack_.isShowWireFrame = false;
	box_.isActive = false;
#endif // _DEBUG

	//必要回数リセット
	needAttackCount_ = 2;

}

void Ogre::SwingDownAttackInit([[maybe_unused]] const States& pre)
{

	animation_.Play("Slash", 0.6f)
		.Loop(false);
	//もしノックバックしたら待機に戻る
	preState_ = States::kIdle;

	//AABBで攻撃判定実装
	aabbAttackCollider_.worldTF.translation = GetSwingDownAttack().attackData.attackPosition;
	aabbAttack_.min = {
		-GetSwingDownAttack().attackData.attackScale,
		-GetSwingDownAttack().attackData.attackScale,
		-GetSwingDownAttack().attackData.attackScale
	};
	aabbAttack_.max = {
		GetSwingDownAttack().attackData.attackScale,
		GetSwingDownAttack().attackData.attackScale,
		GetSwingDownAttack().attackData.attackScale
	};
	//エフェクトフラグリセット
	isActivationParryEffect_ = false;
	//プレイヤーの向きに回転
	RotateTowardsPlayer();
	//攻撃フラグオン
	isAttack_ = true;
	isAttackPhase_ = true;
	GetSwingDownAttack().attackDirection = GetPlayerPosition() - GetPosition();
	GetSwingDownAttack().attackDirection = GetSwingDownAttack().attackDirection.Normalize();
	GetSwingDownAttack().attackData.isPlayedSE = false;

}

void Ogre::SwingDownAttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//パリィエフェクトが発生していないかつ、エフェクトの発生時間を超過したらパリィエフェクト発動
	if (not isActivationParryEffect_ and
		animation_.GetProgress() > GetSwingDownAttack().attackData.startAcceptTime - enemyManager_->GetParryEffectOccurTime()) {
		//パリィエフェクト開始
		StartParryEffect(aabbAttackCollider_.GetWorldPosition());
		isActivationParryEffect_ = true;
	}

	//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
	if (isStartParryEffect_) {
		currentMotionSpeed_ = 0.1f;
		animation_.GetPlayBackSpeed() = currentMotionSpeed_;
		aabbAttackCollider_.isActive = false;
#ifdef _DEBUG
		aabbAttack_.isShowWireFrame = false;
		box_.isActive = false;
#endif // _DEBUG
	}
	//パリィエフェクトが終わったら通常スピードで判定をオンにする
	else if (IsExitParryEffect()) {
		currentMotionSpeed_ = 1.0f;
		animation_.GetPlayBackSpeed() = currentMotionSpeed_;
		//斬撃エフェクト生成
		Quaternion GenerateRotate{};
		GenerateRotate = Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, GetSwingDownAttack().effectParam.rotate.x)
			* Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, GetSwingDownAttack().effectParam.rotate.y)
			* Quaternion::CreateFromAxisAngle({ 0.0f, 0.0f, 1.0f }, GetSwingDownAttack().effectParam.rotate.z);
		slashEffector_.Create(GetSwingDownAttack().effectParam.position, GenerateRotate,
			GetSwingDownAttack().effectParam.scale, GetSwingDownAttack().effectParam.playTime,
			GetSwingDownAttack().effectParam.offset, GetSwingDownAttack().effectParam.color);
	}

	//SEを鳴らす時間に到達したら鳴らす
	if (animation_.GetProgress() >= GetSwingDownAttack().attackData.sePlayTime and
		not GetSwingDownAttack().attackData.isPlayedSE) {
		sePlayer_->PlaySE(GetSwingDownAttack().attackData.seFilePath, "attack1", 0.5f);
		GetSwingDownAttack().attackData.isPlayedSE = true;
	}

	//攻撃受付時間を超過したら判定オフ
	if (animation_.GetProgress() > GetSwingDownAttack().attackData.endAcceptTime and
		aabbAttackCollider_.isActive) {
		aabbAttackCollider_.isActive = false;
#ifdef _DEBUG
		aabbAttack_.isShowWireFrame = false;
		box_.isActive = false;
#endif // _DEBUG
	}
	//開始と終了時間の間だけ判定を付ける
	else if (animation_.GetProgress() >= GetSwingDownAttack().attackData.startAcceptTime and
		animation_.GetProgress() <= GetSwingDownAttack().attackData.endAcceptTime) {
		aabbAttackCollider_.isActive = true;
#ifdef _DEBUG
		aabbAttack_.isShowWireFrame = true;
		box_.isActive = false;
#endif // _DEBUG
	}
	//開始時間未満も判定を付けない
	else {

		//エフェクトが終わって攻撃発生前なら
		if (not isStartParryEffect_ and isActivationParryEffect_ and
			animation_.GetProgress() < GetSwingDownAttack().attackData.startAcceptTime) {

			//間合いの外なら
			if (Vector3::Distance(GetPlayerPosition(), GetPosition()) > GetSwingDownAttack().attackData.attackDistance) {
				//現在向いている向きに進む
				SetPosition(GetPosition() + GetSwingDownAttack().attackDirection *
					GetSwingDownAttack().attackData.moveSpeed * LWP::Info::GetDeltaTimeF() +
					(GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));
			}

		}

		aabbAttackCollider_.isActive = false;
#ifdef _DEBUG
		aabbAttack_.isShowWireFrame = false;
		box_.isActive = false;
#endif // _DEBUG
	}

	//攻撃が終了した時
	if (not animation_.GetPlaying()) {
		isAttack_ = false;
		isAttackPhase_ = false;
		state_.request = States::kIdle;
		return;
	}

}
