#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OniHayhaState;

void OniHayha::RetreatFinalize([[maybe_unused]] const States& pre) {



}

void OniHayha::RetreatInit([[maybe_unused]] const States& pre) {

	SetAnimation("Run", true);
	preState_ = States::kRetreat;

}

void OniHayha::RetreatUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre) {

	//プレイヤーが存在する場合
	if (player_) {

		//待機状態に移行する距離になるまでプレイヤーに向かう
		if (distFromPlayer_ > stateParameter_.retreatParameter.idleDist) {
			state_.request = States::kIdle;
			return;
		}

		Vector3 result{};

		//移動
		result = GetPosition() - GetPlayerPosition();
		//y軸の移動ベクトルを消す
		result.y = 0.0f;
		result = result.Normalize() * LWP::Info::GetDeltaTimeF();

		SetPosition(GetPosition() + result + (GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}


