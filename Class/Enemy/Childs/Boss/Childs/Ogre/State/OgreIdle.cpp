#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OgreState;

void Ogre::IdleFinalize([[maybe_unused]] const States& pre) {

	//移動ステートの時間セット
	stateParameter_.moveParameter.countRunTime = stateParameter_.moveParameter.runTime;

}

void Ogre::IdleInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Idle", true);
	preState_ = States::kIdle;

}

void Ogre::IdleUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//カウントダウン
		if (stateParameter_.idleParameter.countStandTime > 0) {
			stateParameter_.idleParameter.countStandTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

		//0になったら状態切り替え
		if (stateParameter_.idleParameter.countStandTime <= 0) {
			//間合い取り状態に移行
			state_.request = States::kMove;
			return;
		}

	}

}

