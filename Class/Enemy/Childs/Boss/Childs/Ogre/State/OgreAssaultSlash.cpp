#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 連続突撃の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::AssaultSlashFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	isAttack_ = false;
	isAttackPhase_ = false;
	//強攻撃終了時の抽選処理
	EndHeavyAttack();
	SetPosition({ GetPosition().x, 0.0f, GetPosition().z });
}

void Ogre::AssaultSlashInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Idle", true);
	//もしノックバックしたら待機に戻る
	preState_ = States::kIdle;

	//AABB実装
	aabbAttackCollider_.worldTF.translation = GetAssaultSlash().attackData.attackPosition;
	aabbAttackCollider_.worldTF.scale = {
		GetAssaultSlash().attackData.attackScale,
		GetAssaultSlash().attackData.attackScale,
		GetAssaultSlash().attackData.attackScale
	};
	//エフェクトフラグリセット
	isActivationParryEffect_ = false;
	//プレイヤーの向きに回転
	RotateTowardsPlayer();
	//待機フラグオン
	GetAssaultSlash().isWaiting = true;
	GetAssaultSlash().currentTime = 0.0f;
	GetAssaultSlash().currentAttackCount = 0;
	//攻撃フラグオン
	isAttack_ = true;
	isAttackPhase_ = true;
}

void Ogre::AssaultSlashUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	GetAssaultSlash().currentTime += currentMotionSpeed_ * LWP::Info::GetDeltaTimeF();

	//待機中の場合
	if (GetAssaultSlash().isWaiting) {

		//潜っている途中はY座標を下げる
		if (GetAssaultSlash().currentTime < GetAssaultSlash().divingTime) {

			SetPosition(GetPosition() - Vector3{ 0.0f, currentMotionSpeed_ * LWP::Info::GetDeltaTimeF(), 0.0f });

		}

		//合計待機時間を超えたらラッシュ状態に入る
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().divingTime + GetAssaultSlash().stealthWaitingTime) {
			//ラッシュフラグオン
			GetAssaultSlash().isRush = true;
			//待機フラグオフ
			GetAssaultSlash().isWaiting = false;
			//ワープ先セット
			SetAssaultSlashWarpPosition();

		}


	}
	//突撃中の場合
	else if (GetAssaultSlash().isRush) {

		//パリィエフェクトが発生していないかつ、エフェクトの発生時間を超過したらパリィエフェクト発動
		if (not isActivationParryEffect_ and
			GetAssaultSlash().currentTime > GetAssaultSlash().waitingTime - enemyManager_->GetParryEffectOccurTime()) {
			//パリィエフェクト開始
			StartParryEffect(aabbAttackCollider_.GetWorldPosition());
			isActivationParryEffect_ = true;
		}

		//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
		if (isStartParryEffect_) {
			currentMotionSpeed_ = 0.1f;
			animation_.GetPlayBackSpeed() = currentMotionSpeed_;
			aabbAttackCollider_.isActive = false;
			aabbAttack_.isShowWireFrame = false;
		}
		//パリィエフェクトが終わったら通常スピードで判定をオンにする
		else if (IsExitParryEffect()) {
			currentMotionSpeed_ = 1.0f;
			animation_.GetPlayBackSpeed() = currentMotionSpeed_;
		}

		//待機中
		if (GetAssaultSlash().currentTime < GetAssaultSlash().waitingTime) {

			//当たり判定オフ
			aabbAttackCollider_.isActive = false;
			aabbAttack_.isShowWireFrame = false;
		}

		//待機時間を超えたら突進
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().waitingTime) {
			//イージングでポジション補間
			SetPosition(LWP::Utility::Interpolation::Lerp(GetAssaultSlash().attackStartPosition, GetAssaultSlash().attackEndPosition,
				((GetAssaultSlash().currentTime - GetAssaultSlash().waitingTime) / GetAssaultSlash().assaultTime)));
			//当たり判定オン
			aabbAttackCollider_.isActive = true;
			aabbAttack_.isShowWireFrame = true;
		}

		//時間経過後
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().waitingTime + GetAssaultSlash().assaultTime) {
			
			//最大カウントに達していない場合、次の攻撃に移る
			if (GetAssaultSlash().currentAttackCount < GetAssaultSlash().maxAttackCount - 1) {
				//ワープ先セット
				SetAssaultSlashWarpPosition();
				//モーションリセット

				//パリィエフェクトリセット
				isActivationParryEffect_ = false;
				//カウント増加
				GetAssaultSlash().currentAttackCount++;
			}
			//攻撃が終わったら後隙の時間に移行
			else {
				//当たり判定オフ
				aabbAttackCollider_.isActive = false;
				aabbAttack_.isShowWireFrame = false;
				//ポジションを終わりにセット
				SetPosition(GetAssaultSlash().attackEndPosition);
				//ラッシュフラグオフ
				GetAssaultSlash().isRush = false;
				//攻撃フラグオフ
				isAttack_ = false;
				//時間リセット
				GetAssaultSlash().currentTime = 0.0f;
			}
			
		}

	}
	//後隙を出す
	else {

		if (GetAssaultSlash().currentTime >= GetAssaultSlash().gapTime) {
			isAttack_ = false;
			isAttackPhase_ = false;
			//カウントリセット
			GetAssaultSlash().currentTime = 0.0f;
			GetAssaultSlash().currentAttackCount = 0;
			state_.request = States::kIdle;
			return;
		}

	}



}

void Ogre::SetAssaultSlashWarpPosition()
{

	//ワープ先の方向
	LWP::Math::Vector3 moveDrection{};
	//八方向からランダム選出
	int32_t randomNum = LWP::Utility::Random::GenerateInt(0, 7);
	float radian = 45.0f * float(randomNum) * (3.1415f / 180.0f);
	moveDrection = { cosf(radian), 0.0f, sinf(radian) };
	//現在のプレイヤーの座標を元としてmoveDirectionを足す
	SetPosition(GetPlayerPosition() + moveDrection * GetAssaultSlash().leaveDistance);
	//開始地点設定
	GetAssaultSlash().attackStartPosition = GetPlayerPosition() + moveDrection * GetAssaultSlash().leaveDistance;
	//終了地点設定。反対方向のため、-をかける
	GetAssaultSlash().attackEndPosition = GetPlayerPosition() + moveDrection * -GetAssaultSlash().overDistance;
	//時間リセット
	GetAssaultSlash().currentTime = 0.0f;
	//プレイヤーの向きに回転
	RotateTowardsPlayer();

}
