#include "../../../../../../Player/Player.h"
#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OniHayhaState;

void OniHayha::IdleFinalize([[maybe_unused]] const States& pre) {

	

}

void OniHayha::IdleInit([[maybe_unused]] const States& pre)
{
	
	SetAnimation("Idle", true);
	preState_ = States::kIdle;

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
		if (distFromPlayer_ < IdleParameter::retreatDist) {
			//退避状態に移行
			state_.request = States::kRetreat;
			return;
		}

		//0になったら状態切り替え
		if (stateParameter_.idleParameter.countStandTime <= 0) {
			//攻撃状態に移行
			state_.request = States::kAttack;
			return;
		}

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}

