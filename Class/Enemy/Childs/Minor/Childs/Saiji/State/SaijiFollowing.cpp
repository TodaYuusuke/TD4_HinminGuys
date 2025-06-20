#include "../Saiji.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace SaijiState;

void Saiji::FollowingFinalize([[maybe_unused]] const States& pre)
{
}

void Saiji::FollowingInit([[maybe_unused]] const States& pre)
{
	
	SetAnimation("Run", true);
	preState_ = States::kFollowing;

}

void Saiji::FollowingUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//待機状態に移行する距離になるまでプレイヤーに向かう
		if (distFromPlayer_ < FollowingParameter::idleDist) {
			state_.request = States::kIdle;
			return;
		}

		Vector3 velocity{};

		//移動
		velocity = GetPlayerPosition() - GetPosition();
		//y軸の移動ベクトルを消す
		velocity.y = 0.0f;
		velocity = velocity.Normalize() * LWP::Info::GetDeltaTimeF() * parameter_.speed;
		
		SetPosition(GetPosition() + velocity + (GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}
