#include "../../../../../../Player/Player.h"
#include "../Faster.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace FasterState;

void Faster::IdleFinalize(const States& pre) {

	//時間セット
	stateParameter_.spacingParameter.countSpacingTime = SpacingParameter::spacingTime;
	//ランダムな数字を利用して右回りかどうかを決める
	if (LWP::Utility::GenerateRandamNum(0, 1) == 0) {
		stateParameter_.spacingParameter.isClockwise = true;
	}

}

void Faster::IdleInit(const States& pre)
{
	
	SetAnimation("Idle", true);
	preState_ = States::kIdle;

}

void Faster::IdleUpdate(std::optional<States>& req, const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {
		//カウントダウン
		if (stateParameter_.idleParameter.countStandTime > 0) {
			stateParameter_.idleParameter.countStandTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

		//プレイヤーから一定以上の距離離れたら追従モーションに移行
		if (distFromPlayer_ > IdleParameter::followingDist) {
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

