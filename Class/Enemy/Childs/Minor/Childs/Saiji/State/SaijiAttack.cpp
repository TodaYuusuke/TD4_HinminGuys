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
	stateParameter_.idleParameter.countStandTime = stateParameter_.idleParameter.standTime +
		LWP::Utility::Random::GenerateFloat(0.0f, 1.0f);
	animation_.GetPlayBackSpeed() = 1.0f;

}

void Saiji::AttackInit([[maybe_unused]] const States& pre)
{
	
	preState_ = States::kAttack;

	animation_.Play("VerticalAttack", 0.3f)
		.Loop(false);

	aabbAttackCollider_.isActive = false;
	isAttack_ = true;
	//AABB実装
	aabbAttackCollider_.worldTF.translation = stateParameter_.attackParameter.attackPosition;
	aabbAttackCollider_.worldTF.scale = {
		stateParameter_.attackParameter.attackScale,
		stateParameter_.attackParameter.attackScale,
		stateParameter_.attackParameter.attackScale
	};
	//パリィエフェクトフラグリセット
	isActivationParryEffect_ = false;

	stateParameter_.attackParameter.currentFreezingTime = 0.0f;

}

void Saiji::AttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//パリィエフェクトが発生していないかつ、エフェクトの発生時間を超過したらパリィエフェクト発動
	if (not isActivationParryEffect_ and
		animation_.GetProgress() > stateParameter_.attackParameter.startAcceptTime - enemyManager_->GetParryEffectOccurTime()) {
		//パリィエフェクト開始
		StartParryEffect(aabbAttackCollider_.GetWorldPosition());
		isActivationParryEffect_ = true;
	}

	//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
	if (isStartParryEffect_) {
		animation_.GetPlayBackSpeed() = 0.1f;
		aabbAttackCollider_.isActive = false;
	}
	//パリィエフェクトが終わったら通常スピードで判定をオンにする
	else if(IsExitParryEffect()) {
		animation_.GetPlayBackSpeed() = 1.0f;
	}

	//攻撃受付時間を超過したら判定オフ
	if (animation_.GetProgress() > stateParameter_.attackParameter.endAcceptTime and aabbAttackCollider_.isActive) {
		isAttack_ = false;
		aabbAttackCollider_.isActive = false;

# ifdef _DEBUG
		aabbAttack_.isShowWireFrame = false;
#endif // _DEBUG
		
	}
	//開始と終了時間の間だけ判定を付ける
	else if (animation_.GetProgress() >= stateParameter_.attackParameter.startAcceptTime and
		animation_.GetProgress() <= stateParameter_.attackParameter.endAcceptTime) {
		aabbAttackCollider_.isActive = true;

# ifdef _DEBUG
		aabbAttack_.isShowWireFrame = true;
#endif // _DEBUG
		
	}
	//開始時間未満も判定を付けない
	else if(animation_.GetProgress() < stateParameter_.attackParameter.startAcceptTime) {
		aabbAttackCollider_.isActive = false;

# ifdef _DEBUG
		aabbAttack_.isShowWireFrame = false;
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

	/*SetPosition(GetPosition() + (GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));*/

}

