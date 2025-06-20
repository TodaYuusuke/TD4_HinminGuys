#include "../../../../../../Player/Player.h"
#include "../Faster.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace FasterState;

void Faster::MoveFinalize([[maybe_unused]] const States& pre) {

	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = IdleParameter::standTime;

}

void Faster::MoveInit([[maybe_unused]] const States& pre)
{
	
	SetAnimation("Run", true);
	preState_ = States::kMove;

}

void Faster::MoveUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//カウントダウン
		if (stateParameter_.moveParameter.countRunTime > 0) {
			stateParameter_.moveParameter.countRunTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

		//プレイヤーとの距離が近く、誰も攻撃していなかったら攻撃
		if (Vector3::Distance(GetPlayerPosition(), GetPosition()) < MoveParameter::attackDist) {

			//誰も攻撃していない状態で、攻撃待機中の敵もいない場合、攻撃にそのまま移行
			if (not enemyManager_->IsAnyAttack() and
				WaitingForAttackParameter::attackCount == WaitingForAttackParameter::nextAttackCount) {
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
			SetIsAttackPhase(false);
			state_.request = States::kIdle;
			return;

		}

		//移動
		stateParameter_.moveParameter.direction = GetPlayerPosition() - GetPosition();
		//y軸の移動ベクトルを消す
		stateParameter_.moveParameter.direction.y = 0.0f;

		stateParameter_.moveParameter.direction =
			stateParameter_.moveParameter.direction.Normalize() *
			stateParameter_.moveParameter.speed * LWP::Info::GetDeltaTimeF();

		SetPosition(GetPosition() + stateParameter_.moveParameter.direction +
			(GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}

