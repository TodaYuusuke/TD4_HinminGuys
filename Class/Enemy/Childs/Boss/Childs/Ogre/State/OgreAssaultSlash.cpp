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
	animation_.GetPlayBackSpeed() = 1.0f;
	//強攻撃終了時の抽選処理
	EndHeavyAttack();
	SetPosition({ GetPosition().x, 0.0f, GetPosition().z });
	aabbAttackCollider_.isActive = false;
#ifdef _DEBUG
	aabbAttack_.isShowWireFrame = false;
	box_.isActive = false;
#endif // _DEBUG

	model_.worldTF.translation.y = 0.0f;

}

void Ogre::AssaultSlashInit([[maybe_unused]] const States& pre)
{

	animation_.Play("ShadowHide", 0.2f)
		.Loop(false);
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
	//SE発生フラグリセット
	GetAssaultSlash().attackData.isPlayedSE = false;

	// 出現演出開始
	enemyManager_->GetParticles()->CreateEnemySpawnParticles(
		stateParameter_.spawnParameter.appearanceTime, Vector3{ model_.worldTF.translation.x, 0.0f, model_.worldTF.translation.z });

}

void Ogre::AssaultSlashUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	GetAssaultSlash().currentTime += currentMotionSpeed_ * LWP::Info::GetDeltaTimeF();

	//待機中の場合
	if (GetAssaultSlash().isWaiting) {

		//ロックオン不可
		if (player_) {
			player_->GetSystemManager()->GetLockOnSystem()->Reset();
		}

		//SEを鳴らす時間に到達したら鳴らす
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().startSEPlayTime and
			not GetAssaultSlash().attackData.isPlayedSE) {
			sePlayer_->PlaySE("enemy/ogre/piercing.mp3", "piercing", 0.5f);
			GetAssaultSlash().attackData.isPlayedSE = true;
		}

		//潜っている途中はY座標を下げる
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().diveWaitingTime and 
			GetAssaultSlash().currentTime < GetAssaultSlash().diveWaitingTime + GetAssaultSlash().divingTime) {

			SetPosition(GetPosition() - Vector3{ 0.0f, currentMotionSpeed_ * GetAssaultSlash().diveSpeed * LWP::Info::GetDeltaTimeF(), 0.0f });

		}

		//合計待機時間を超えたらラッシュ状態に入る
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().diveWaitingTime + GetAssaultSlash().divingTime + GetAssaultSlash().stealthWaitingTime) {
			//ラッシュフラグオン
			GetAssaultSlash().isRush = true;
			//待機フラグオフ
			GetAssaultSlash().isWaiting = false;
			//モーションリセット
			animation_.Play("RushSlash", 0.2f)
				.Loop(false);
			//ワープ先セット
			SetAssaultSlashWarpPosition();

		}


	}
	//突撃中の場合
	else if (GetAssaultSlash().isRush) {

		//ロックオン不可
		if (player_) {
			player_->GetSystemManager()->GetLockOnSystem()->Reset();
		}

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
			GenerateRotate = Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, GetAssaultSlash().effectParam.rotate.x)
				* Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, GetAssaultSlash().effectParam.rotate.y)
				* Quaternion::CreateFromAxisAngle({ 0.0f, 0.0f, 1.0f }, GetAssaultSlash().effectParam.rotate.z);
			slashEffector_.Create(GetAssaultSlash().effectParam.position, GenerateRotate,
				GetAssaultSlash().effectParam.scale, GetAssaultSlash().effectParam.playTime,
				GetAssaultSlash().effectParam.offset, GetAssaultSlash().effectParam.color);
		}

		//SEを鳴らす時間に到達したら鳴らす
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().attackData.sePlayTime and
			not GetAssaultSlash().attackData.isPlayedSE) {
			sePlayer_->PlaySE(GetAssaultSlash().attackData.seFilePath, "attack2", 0.5f);
			GetAssaultSlash().attackData.isPlayedSE = true;
		}

		//待機中
		if (GetAssaultSlash().currentTime < GetAssaultSlash().waitingTime) {
			//アニメーション一時停止
			animation_.Pause();
			//当たり判定オフ
			aabbAttackCollider_.isActive = false;
#ifdef _DEBUG
			aabbAttack_.isShowWireFrame = false;
			box_.isActive = false;
#endif // _DEBUG
		}

		//待機時間を超えたら突進
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().waitingTime) {
			//アニメーション再開
			animation_.Resume();
			animation_.GetPlayBackSpeed() = 2.0f;


			//イージングでポジション補間(壁貫通)
			UnlimitedSetPosition(LWP::Utility::Interpolation::Lerp(GetAssaultSlash().attackStartPosition, GetAssaultSlash().attackEndPosition,
				((GetAssaultSlash().currentTime - GetAssaultSlash().waitingTime) / GetAssaultSlash().assaultTime)));
			//当たり判定オン
			aabbAttackCollider_.isActive = true;
#ifdef _DEBUG
			aabbAttack_.isShowWireFrame = true;
			box_.isActive = false;
#endif // _DEBUG
		}

		//時間経過後
		if (GetAssaultSlash().currentTime >= GetAssaultSlash().waitingTime + GetAssaultSlash().assaultTime) {
			
			//最大カウントに達していない場合、次の攻撃に移る
			if (GetAssaultSlash().currentAttackCount < GetAssaultSlash().maxAttackCount - 1) {
				//ワープ先セット
				SetAssaultSlashWarpPosition();
				//モーションリセット
				animation_.Play("RushSlash", 0.2f)
					.Loop(false);
				//パリィエフェクトリセット
				isActivationParryEffect_ = false;
				//カウント増加
				GetAssaultSlash().currentAttackCount++;
			}
			//攻撃が終わったら後隙の時間に移行
			else {
				animation_.GetPlayBackSpeed() = 1.0f;
				//当たり判定オフ
				aabbAttackCollider_.isActive = false;
#ifdef _DEBUG
				aabbAttack_.isShowWireFrame = false;
				box_.isActive = false;
#endif // _DEBUG
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
	LWP::Math::Vector3 moveDirection{};

	//五回目の攻撃の場合
	if (GetAssaultSlash().currentAttackCount >= GetAssaultSlash().maxAttackCount - 2) {
		//中心座標とプレイヤーの座標から移動方向を計算
		moveDirection = Vector3{ 0.0f,0.0f,0.0f } + GetPlayerPosition();
		moveDirection = moveDirection.Normalize();

	}
	//四回目までの場合
	else {
		//八方向からランダム選出
		int32_t randomNum = LWP::Utility::Random::GenerateInt(0, 7);
		float radian = 45.0f * float(randomNum) * (3.1415f / 180.0f);
		moveDirection = { cosf(radian), 0.0f, sinf(radian) };
	}
	
	//現在のプレイヤーの座標を元としてmoveDirectionを足す
	UnlimitedSetPosition(GetPlayerPosition() + moveDirection * GetAssaultSlash().leaveDistance);
	//開始地点設定
	GetAssaultSlash().attackStartPosition = GetPosition();
	//終了地点設定。反対方向のため、-をかける
	GetAssaultSlash().attackEndPosition = GetPlayerPosition() + moveDirection * -GetAssaultSlash().overDistance;
	//時間リセット
	GetAssaultSlash().currentTime = 0.0f;
	//プレイヤーの向きに回転
	RotateTowardsPlayer();

	GetAssaultSlash().attackData.isPlayedSE = false;

}
