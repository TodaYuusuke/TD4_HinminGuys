#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 移動の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::MoveFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = stateParameter_.idleParameter.standTime;
	//デフォの移動速度セット
	parameter_.speed = 1.0f;

}

void Ogre::MoveInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Run", true);
	preState_ = States::kMove;
	//移動速度をセット
	parameter_.speed = stateParameter_.moveParameter.defaultSpeed;

	//次の攻撃ステートが存在しない場合
	

}

void Ogre::MoveUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//カウントダウン
		if (stateParameter_.moveParameter.countRunTime > 0) {
			stateParameter_.moveParameter.countRunTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

		//次の攻撃ステートが連続突撃の場合、そのまま移行
		if (nextAttackState_ == States::kAssaultSlash) {
			state_.request = nextAttackState_;
		}

		//カウントが0以上の場合、通常スピード
		if (stateParameter_.moveParameter.countRunTime > 0.0f) {
			parameter_.speed = stateParameter_.moveParameter.defaultSpeed;
		}
		//0以下ならブースト
		else {
			parameter_.speed = stateParameter_.moveParameter.boostSpeed;
		}

		//プレイヤーとの距離が近い場合
		if (Vector3::Distance(GetPlayerPosition(), GetPosition()) < stateParameter_.moveParameter.attackDist) {

			state_.request = nextAttackState_;

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

