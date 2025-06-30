#include "../../../../../../Player/Player.h"
#include "../Saiji.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace SaijiState;

void Saiji::MoveFinalize([[maybe_unused]] const States& pre) {

	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = stateParameter_.idleParameter.standTime;
	//デフォの移動速度セット
	parameter_.speed = 1.0f;

}

void Saiji::MoveInit([[maybe_unused]] const States& pre)
{
	
	SetAnimation("Run", true);
	preState_ = States::kMove;
	//移動速度をセット
	parameter_.speed = stateParameter_.moveParameter.defaultSpeed;

}

void Saiji::MoveUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//カウントダウン
		if (stateParameter_.moveParameter.countRunTime > 0) {
			stateParameter_.moveParameter.countRunTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

		//プレイヤーとの距離が近く、誰も攻撃していなかったら攻撃
		if (Vector3::Distance(GetPlayerPosition(), GetPosition()) < stateParameter_.moveParameter.attackDist) {

			//誰も攻撃していない状態で、攻撃待機中の敵もいない場合、攻撃にそのまま移行
			if (not enemyManager_->IsAnyAttackWithinType(attackType_) and
				enemyManager_->shortAssignAttackID == enemyManager_->shortNextAttackID) {
				//攻撃状態に移行
				state_.request = States::kAttack;
			}
			else {
				//攻撃待機状態に移行
				state_.request = States::kWaitingForAttack;
			}

			return;
		}

		//0になったら行動を切り替える
		if (stateParameter_.moveParameter.countRunTime <= 0) {

			//待機状態に移行
			isAttackPhase_ = false;
			state_.request = States::kIdle;
			return;

		}

		//移動
		stateParameter_.moveParameter.direction = GetPlayerPosition() - GetPosition();
		//y軸の移動ベクトルを消す
		stateParameter_.moveParameter.direction.y = 0.0f;

		stateParameter_.moveParameter.direction =
			stateParameter_.moveParameter.direction.Normalize() *
			parameter_.speed * LWP::Info::GetDeltaTimeF();

		SetPosition(GetPosition() + stateParameter_.moveParameter.direction +
			(GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}

