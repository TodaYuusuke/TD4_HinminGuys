#include "../../../../../../Player/Player.h"
#include "../Saiji.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace SaijiState;

void Saiji::IdleFinalize([[maybe_unused]] const States& pre) {

	//時間セット
	stateParameter_.spacingParameter.countSpacingTime = stateParameter_.spacingParameter.spacingTime + 
		LWP::Utility::Random::GenerateFloat(-0.3f, 0.3f);
	//ランダムな数字を利用して右回りかどうかを決める
	if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
		stateParameter_.spacingParameter.isClockwise = true;
	}
	else {
		stateParameter_.spacingParameter.isClockwise = false;
	}

}

void Saiji::IdleInit([[maybe_unused]] const States& pre)
{
	
	animation_.Play("Idle", 0.6f)
		.Loop(true);
	preState_ = States::kIdle;

}

void Saiji::IdleUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {
		//カウントダウン
		if (stateParameter_.idleParameter.countStandTime > 0) {
			stateParameter_.idleParameter.countStandTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

		//プレイヤーから一定以上の距離離れたら追従モーションに移行
		if (distFromPlayer_ > stateParameter_.idleParameter.followingDist) {
			state_.request = States::kFollowing;
			return;
		}

		//0になったら状態切り替え
		if (stateParameter_.idleParameter.countStandTime <= 0) {
			//間合い取り状態に移行
			state_.request = States::kSpacing;
			return;
		}

	}

}

