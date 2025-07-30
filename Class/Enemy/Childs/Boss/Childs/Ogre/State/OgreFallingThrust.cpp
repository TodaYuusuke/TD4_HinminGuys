#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 落下攻撃の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::FallingThrustFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	isAttack_ = false;
	isAttackPhase_ = false;
	//中攻撃終了時の抽選処理
	EndMediumAttack();
	sphereCollider_.isActive = false;
#ifdef _DEBUG
	sphere_.isShowWireFrame = false;
#endif // _DEBUG

}

void Ogre::FallingThrustInit([[maybe_unused]] const States& pre)
{

	animation_.Play("Jump", 0.2f)
		.Loop(false);
	//もしノックバックしたら待機に戻る
	preState_ = States::kIdle;
	//球で実装
	sphereCollider_.worldTF.translation = GetFallingThrust().attackData.attackPosition;
	sphereCollider_.worldTF.scale = {
		GetFallingThrust().attackData.attackScale,
		GetFallingThrust().attackData.attackScale,
		GetFallingThrust().attackData.attackScale
	};
	//エフェクトフラグリセット
	isActivationParryEffect_ = false;
	//プレイヤーの向きに回転
	RotateTowardsPlayer();
	//攻撃フラグオン
	isAttack_ = true;
	isAttackPhase_ = true;
	//ジャンプ先座標をセット
	GetFallingThrust().jumpingPosition = GetPlayerPosition() + Vector3{ 0.0f,GetFallingThrust().jumpingHeight, 0.0f };
	//始点座標をセット
	GetFallingThrust().startPosition = GetPosition();
	//終点座標をセット
	GetFallingThrust().endPosition = GetPlayerPosition();
	sphereCollider_.isActive = false;
#ifdef _DEBUG
	sphere_.isShowWireFrame = false;
#endif // _DEBUG
	GetFallingThrust().currentTime = 0.0f;
	cautionCircle_.isActive = true;
	cautionCircle_.worldTF.scale = {
		GetFallingThrust().attackData.attackScale,
		GetFallingThrust().attackData.attackScale,
		GetFallingThrust().attackData.attackScale
	};
	cautionCircle_.worldTF.translation = GetPlayerPosition() + Vector3{ 0.0f,0.1f,0.0f };

}

void Ogre::FallingThrustUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{
	//時間計測
	GetFallingThrust().currentTime += currentMotionSpeed_ * LWP::Info::GetDeltaTimeF();

	//パリィエフェクトが発生していないかつ、エフェクトの発生時間を超過したらパリィエフェクト発動
	if (not isActivationParryEffect_ and
		GetFallingThrust().currentTime > GetFallingThrust().jumpingTime + GetFallingThrust().fallWaitingTime +
		GetFallingThrust().fallingTime - enemyManager_->GetParryEffectOccurTime()) {
		//パリィエフェクト開始
		StartParryEffect(sphereCollider_.GetWorldPosition());
		isActivationParryEffect_ = true;
	}

	//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
	if (isStartParryEffect_) {
		currentMotionSpeed_ = 0.1f;
		animation_.GetPlayBackSpeed() = currentMotionSpeed_;
		sphereCollider_.isActive = false;
#ifdef _DEBUG
		sphere_.isShowWireFrame = false;
#endif // _DEBUG
	}
	//パリィエフェクトが終わったら通常スピードで判定をオンにする
	else if (IsExitParryEffect()) {
		currentMotionSpeed_ = 1.0f;
		animation_.GetPlayBackSpeed() = currentMotionSpeed_;
	}

	//ジャンプ前待機時間
	if (GetFallingThrust().currentTime < GetFallingThrust().jumpWaitingTime) {
		SetPosition(GetFallingThrust().startPosition);
	}
	//ジャンプ中
	else if (GetFallingThrust().currentTime < GetFallingThrust().jumpingTime) {

		//ジャンプ座標までセット
		SetPosition(LWP::Utility::Interpolation::Lerp(GetFallingThrust().startPosition, GetFallingThrust().jumpingPosition,
			(GetFallingThrust().currentTime - GetFallingThrust().jumpWaitingTime) / GetFallingThrust().jumpingTime));

		//ロックオン不可
		if (player_) {
			player_->GetSystemManager()->GetLockOnSystem()->Reset();
		}

	}
	//ジャンプ後の待機時間
	else if (GetFallingThrust().currentTime < GetFallingThrust().jumpingTime + GetFallingThrust().fallWaitingTime) {

		SetPosition(GetFallingThrust().jumpingPosition);

		//アニメーションが再生中でない場合、次のアニメーションに移行
		if (not animation_.GetPlaying()) {
			animation_.Play("FallAttack", 0.2f)
				.Loop(false);
		}

	}
	//落下中
	else if (GetFallingThrust().currentTime < GetFallingThrust().jumpingTime + GetFallingThrust().fallWaitingTime + 
		GetFallingThrust().fallingTime) {

		//落下座標までセット
		SetPosition(LWP::Utility::Interpolation::Lerp(GetFallingThrust().jumpingPosition, GetFallingThrust().endPosition,
			(GetFallingThrust().currentTime - 
				GetFallingThrust().jumpingTime - GetFallingThrust().fallWaitingTime) / GetFallingThrust().fallingTime));

	}
	//落下後
	else if (GetFallingThrust().currentTime < GetFallingThrust().jumpingTime + GetFallingThrust().fallWaitingTime + 
		GetFallingThrust().fallingTime + GetFallingThrust().gapTime) {

		//攻撃発生時間
		if (GetFallingThrust().currentTime < GetFallingThrust().jumpingTime + GetFallingThrust().fallWaitingTime +
			GetFallingThrust().fallingTime + GetFallingThrust().attackTime and not isStartParryEffect_) {
			sphereCollider_.isActive = true;
#ifdef _DEBUG
			sphere_.isShowWireFrame = true;
#endif // _DEBUG
		}
		else {
			sphereCollider_.isActive = false;
#ifdef _DEBUG
			sphere_.isShowWireFrame = false;
#endif // _DEBUG
			cautionCircle_.isActive = false;
		}

		SetPosition(GetFallingThrust().endPosition);

	}
	//攻撃終了
	else {
		isAttack_ = false;
		isAttackPhase_ = false;
		sphereCollider_.isActive = false;
#ifdef _DEBUG
		sphere_.isShowWireFrame = false;
#endif // _DEBUG
		
		GetFallingThrust().currentTime = 0.0f;
		state_.request = States::kIdle;
		return;
	}

}
