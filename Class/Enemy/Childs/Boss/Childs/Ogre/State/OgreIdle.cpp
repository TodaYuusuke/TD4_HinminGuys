#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 待機の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::IdleFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	//移動ステートの時間セット
	stateParameter_.moveParameter.countRunTime = stateParameter_.moveParameter.runTime;

}

void Ogre::IdleInit([[maybe_unused]] const States& pre)
{

	animation_.Play("Idle", 0.6f)
		.Loop(true);
	preState_ = States::kIdle;
	//必要回数リセット
	needAttackCount_ = 2;

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

			//スポーンラインを下回った時敵出現
			if ((parameter_.hp <= parameter_.maxHp * stateParameter_.spawnEnemy.firstSpawnLine and
				stateParameter_.spawnEnemy.currentCount == 0) or 
				(parameter_.hp <= parameter_.maxHp * stateParameter_.spawnEnemy.secondSpawnLine and
					stateParameter_.spawnEnemy.currentCount == 1)) {
				//スポーン状態に移行
				state_.request = States::kSpawnEnemy;
				return;
			}

			//移動状態に移行
			state_.request = States::kMove;
			return;
		}

	}

}

