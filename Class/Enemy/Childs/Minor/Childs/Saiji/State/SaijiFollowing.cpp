#include "../Saiji.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace SaijiState;

void Saiji::FollowingFinalize(const States& pre)
{
}

void Saiji::FollowingInit(const States& pre)
{
	
	SetAnimation("Run", true);
	preState_ = States::kFollowing;

}

void Saiji::FollowingUpdate(std::optional<States>& req, const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//待機状態に移行する距離になるまでプレイヤーに向かう
		if (distFromPlayer_ < FollowingParameter::idleDist) {
			state_.request = States::kIdle;
			return;
		}

		Vector3 result{};

		//移動
		result = GetPlayerPosition() - GetPosition();
		//y軸の移動ベクトルを消す
		result.y = 0.0f;
		result = result.Normalize() * LWP::Info::GetDeltaTime();
		
		SetPosition(GetPosition() + result + (GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}
