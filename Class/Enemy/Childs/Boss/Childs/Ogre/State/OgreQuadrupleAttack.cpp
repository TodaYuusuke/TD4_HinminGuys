#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 四連攻撃の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::QuadrupleAttackFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	//カウントリセット
	GetQuadrupleAttack().currentAttackCount = 0;
	isAttack_ = false;

}

void Ogre::QuadrupleAttackInit([[maybe_unused]] const States& pre)
{

	SetAnimation("LightAttack1", false);
	//もしノックバックしたら待機に戻る
	preState_ = States::kIdle;
	//カウントリセット
	GetQuadrupleAttack().currentAttackCount = 0;

	//AABBで攻撃判定実装
	aabbAttackCollider_.worldTF.translation = GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackPosition;
	aabbAttackCollider_.worldTF.scale = {
		GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackScale,
		GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackScale,
		GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackScale
	};
	//エフェクトフラグリセット
	isActivationParryEffect_ = false;
	//移動方向を決める
	GetQuadrupleAttack().attackDirection = GetPlayerPosition() - GetPosition();
	GetQuadrupleAttack().attackDirection = GetQuadrupleAttack().attackDirection.Normalize();
	//プレイヤーの向きに回転
	RotateTowardsPlayer();

}

void Ogre::QuadrupleAttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//カウントが最大数に達していない場合
	if (GetQuadrupleAttack().currentAttackCount < GetQuadrupleAttack().kMaxAttackCount) {

		//パリィエフェクトが発生していないかつ、エフェクトの発生時間を超過したらパリィエフェクト発動
		if (not isActivationParryEffect_ and
			animation_.GetProgress() > GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].startAcceptTime - enemyManager_->GetParryEffectOccurTime()) {
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

		//攻撃受付時間を超過したら判定オフ
		if (animation_.GetProgress() > GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].endAcceptTime and
			aabbAttackCollider_.isActive) {
			aabbAttackCollider_.isActive = false;
			aabbAttack_.isShowWireFrame = false;
		}
		//開始と終了時間の間だけ判定を付ける
		else if (animation_.GetProgress() >= GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].startAcceptTime and
			animation_.GetProgress() <= GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].endAcceptTime) {
			aabbAttackCollider_.isActive = true;
			aabbAttack_.isShowWireFrame = true;
		}
		//開始時間未満も判定を付けない
		else {

			//エフェクトが終わって攻撃発生前なら
			if (not isStartParryEffect_ and isActivationParryEffect_ and 
				animation_.GetProgress() < GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].startAcceptTime) {

				//間合いの外なら
				if (Vector3::Distance(GetPlayerPosition(), GetPosition()) > GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackDistance) {
					//現在向いている向きに進む
					SetPosition(GetPosition() + GetQuadrupleAttack().attackDirection * 
						GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].moveSpeed * LWP::Info::GetDeltaTimeF() +
						(GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));
				}
				
			}

			aabbAttackCollider_.isActive = false;
			aabbAttack_.isShowWireFrame = false;
		}

		//アニメーションが強制終了時間を超えたら
		if (animation_.GetProgress() >= GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].endMotionTime) {
			//アニメーション終了
			animation_.Stop();
			//攻撃カウント増加
			GetQuadrupleAttack().currentAttackCount++;
			//カウントが最大攻撃回数以下なら、次の攻撃をセット
			if (GetQuadrupleAttack().currentAttackCount < GetQuadrupleAttack().kMaxAttackCount) {
				//次の攻撃の名前
				std::string nextAttackName = "LightAttack";
				nextAttackName += std::to_string(GetQuadrupleAttack().currentAttackCount + 1);
				//アニメーションセット
				SetAnimation(nextAttackName, false);

				//攻撃判定をセット
				aabbAttackCollider_.worldTF.translation = GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackPosition;
				aabbAttackCollider_.worldTF.scale = {
					GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackScale,
					GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackScale,
					GetQuadrupleAttack().multipleAttackData[GetQuadrupleAttack().currentAttackCount].attackScale
				};
				//一時的に判定をfalseに
				aabbAttackCollider_.isActive = false;
				//パリィエフェクトの発生リセット
				isActivationParryEffect_ = false;
				//移動方向を決める
				GetQuadrupleAttack().attackDirection = GetPlayerPosition() - GetPosition();
				GetQuadrupleAttack().attackDirection = GetQuadrupleAttack().attackDirection.Normalize();
				//プレイヤーの向きに回転
				RotateTowardsPlayer();

			}

		}

	}

	//四連攻撃が終了した時
	if (GetQuadrupleAttack().currentAttackCount >= GetQuadrupleAttack().kMaxAttackCount and
		not animation_.GetPlaying()) {
		isAttack_ = false;
		//カウントリセット
		GetQuadrupleAttack().currentAttackCount = 0;
		state_.request = States::kIdle;
		return;
	}

}
