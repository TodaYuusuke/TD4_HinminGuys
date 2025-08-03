#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 回転攻撃の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::RotatingSlashFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	isAttack_ = false;
	isAttackPhase_ = false;
	//弱攻撃終了時の抽選処理
	EndLightAttack();
	sphereCollider_.isActive = false;
#ifdef _DEBUG
	sphere_.isShowWireFrame = false;
	tmpSphere_.isActive = false;
#endif // _DEBUG

	//必要回数リセット
	needAttackCount_ = 2;

}

void Ogre::RotatingSlashInit([[maybe_unused]] const States& pre)
{

	animation_.Play("RotatingSlash", 0.6f)
		.Loop(false);
	//もしノックバックしたら待機に戻る
	preState_ = States::kIdle;

	//球で攻撃判定実装
	sphereCollider_.worldTF.translation = GetRotatingSlash().attackData.attackPosition;
	/*sphereCollider_.worldTF.scale = {
		GetRotatingSlash().attackData.attackScale,
		GetRotatingSlash().attackData.attackScale,
		GetRotatingSlash().attackData.attackScale
	};*/
	sphere_.radius = GetRotatingSlash().attackData.attackScale;
	//エフェクトフラグリセット
	isActivationParryEffect_ = false;
	//プレイヤーの向きに回転
	RotateTowardsPlayer();
	//攻撃フラグオン
	isAttack_ = true;
	isAttackPhase_ = true;
	GetRotatingSlash().attackData.isPlayedSE = false;

#ifdef _DEBUG
	tmpSphere_.worldTF.scale = Vector3{ 1.0f,1.0f,1.0f } * sphere_.radius;
#endif // _DEBUG


}

void Ogre::RotatingSlashUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//パリィエフェクトが発生していないかつ、エフェクトの発生時間を超過したらパリィエフェクト発動
	if (not isActivationParryEffect_ and
		animation_.GetProgress() > GetRotatingSlash().attackData.startAcceptTime - enemyManager_->GetParryEffectOccurTime()) {
		//パリィエフェクト開始
		StartParryEffect(aabbAttackCollider_.GetWorldPosition());
		isActivationParryEffect_ = true;
	}

	//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
	if (isStartParryEffect_) {
		currentMotionSpeed_ = 0.1f;
		animation_.GetPlayBackSpeed() = currentMotionSpeed_;
		sphereCollider_.isActive = false;
#ifdef _DEBUG
		sphere_.isShowWireFrame = false;
		tmpSphere_.isActive = false;
#endif // _DEBUG
	}
	//パリィエフェクトが終わったら通常スピードで判定をオンにする
	else if (IsExitParryEffect()) {
		currentMotionSpeed_ = 1.0f;
		animation_.GetPlayBackSpeed() = currentMotionSpeed_;
		//斬撃エフェクト生成
		Quaternion GenerateRotate{};
		GenerateRotate = Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, GetRotatingSlash().effectParam.rotate.x)
			* Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, GetRotatingSlash().effectParam.rotate.y)
			* Quaternion::CreateFromAxisAngle({ 0.0f, 0.0f, 1.0f }, GetRotatingSlash().effectParam.rotate.z);
		slashEffector_.Create(GetRotatingSlash().effectParam.position, GenerateRotate,
			GetRotatingSlash().effectParam.scale, GetRotatingSlash().effectParam.playTime,
			GetRotatingSlash().effectParam.offset, GetRotatingSlash().effectParam.color);
	}

	//SEを鳴らす時間に到達したら鳴らす
	if (animation_.GetProgress() >= GetRotatingSlash().attackData.sePlayTime and
		not GetRotatingSlash().attackData.isPlayedSE) {
		sePlayer_->PlaySE(GetRotatingSlash().attackData.seFilePath, "attack3", 0.5f);
		GetRotatingSlash().attackData.isPlayedSE = true;
	}

	//攻撃受付時間を超過したら判定オフ
	if (animation_.GetProgress() > GetRotatingSlash().attackData.endAcceptTime and
		sphereCollider_.isActive) {
		sphereCollider_.isActive = false;
#ifdef _DEBUG
		sphere_.isShowWireFrame = false;
		tmpSphere_.isActive = false;
#endif // _DEBUG
	}
	//開始と終了時間の間だけ判定を付ける
	else if (animation_.GetProgress() >= GetRotatingSlash().attackData.startAcceptTime and
		animation_.GetProgress() <= GetRotatingSlash().attackData.endAcceptTime) {
		sphereCollider_.isActive = true;
#ifdef _DEBUG
		sphere_.isShowWireFrame = true;
		tmpSphere_.isActive = false;
#endif // _DEBUG
	}
	//開始時間未満も判定を付けない
	else {
		sphereCollider_.isActive = false;
#ifdef _DEBUG
		sphere_.isShowWireFrame = false;
		tmpSphere_.isActive = false;
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
