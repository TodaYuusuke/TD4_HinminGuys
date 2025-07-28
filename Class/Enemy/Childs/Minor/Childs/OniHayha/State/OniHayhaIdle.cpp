#include "../../../../../../Player/Player.h"
#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OniHayhaState;

void OniHayha::IdleFinalize([[maybe_unused]] const States& pre) {

}

void OniHayha::IdleInit([[maybe_unused]] const States& pre)
{
	
	animation_.Play("Idle", 0.6f)
		.Loop(true);
	preState_ = States::kIdle;
	//攻撃状態解除
	isAttack_ = false;

}

void OniHayha::IdleUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {
		//カウントダウン
		if (stateParameter_.idleParameter.countStandTime > 0) {
			stateParameter_.idleParameter.countStandTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

		//プレイヤーが近づいてきたらゆっくり後ずさりして距離を取る
		if (distFromPlayer_ < stateParameter_.idleParameter.retreatDist) {
			//退避状態に移行
			state_.request = States::kRetreat;
			return;
		}

		//0になったら状態切り替え
		if (stateParameter_.idleParameter.countStandTime <= 0) {

			//誰も攻撃していない状態で、攻撃待機中の敵もいない場合、狙い撃ちにそのまま移行
			if (not enemyManager_->IsAnyAttackWithinType(attackType_) and
				enemyManager_->longAssignAttackID == enemyManager_->longNextAttackID) {
				//狙い撃ち状態の時間セット
				stateParameter_.aimingParameter.countAimingTime = stateParameter_.aimingParameter.aimingTime;
				//狙い撃ち状態に移行
				state_.request = States::kAiming;
			}
			else {
				//攻撃待機状態に移行
				state_.request = States::kWaitingForAttack;
			}

			return;
		}

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}

