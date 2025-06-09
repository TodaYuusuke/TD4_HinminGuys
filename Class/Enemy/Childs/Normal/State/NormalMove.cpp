#include "../../../../Player/Player.h"
#include "../Normal.h"
#include "../../../EnemyManager.h"

using namespace LWP::Math;

void Normal::MoveFinalize(const States& pre) {

	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = IdleParameter::standTime_;

}

void Normal::MoveInit(const States& pre)
{
	
	SetAnimation("Run", true);
	preState_ = States::kNormalMove;

}

void Normal::MoveUpdate(std::optional<States>& req, const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//カウントダウン
		if (stateParameter_.moveParameter.countRunTime > 0) {
			stateParameter_.moveParameter.countRunTime--;
		}

		//プレイヤーとの距離が近く、誰も攻撃していなかったら攻撃
		if (Vector3::Distance(GetPlayerPosition(), GetPosition()) < MoveParameter::attackDist_) {

			//誰も攻撃していない状態で、攻撃待機中の敵もいない場合、攻撃にそのまま移行
			if (not enemyManager_->IsAnyAttack() and
				WaitingForAttackParameter::attackCount_ == WaitingForAttackParameter::nextAttackCount_) {
				//攻撃状態に移行
				state_.request = States::kNormalAttack;
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
			state_.request = States::kNormalIdle;
			return;

		}

		//移動
		stateParameter_.moveParameter.velocity = GetPlayerPosition() - GetPosition();
		//y軸の移動ベクトルを消す
		stateParameter_.moveParameter.velocity.y = 0.0f;

		stateParameter_.moveParameter.velocity =
			stateParameter_.moveParameter.velocity.Normalize() * LWP::Info::GetDeltaTime();

		SetPosition(GetPosition() + stateParameter_.moveParameter.velocity +
			(GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}

