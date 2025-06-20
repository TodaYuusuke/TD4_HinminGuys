#include "../../../../../../Player/Player.h"
#include "../Faster.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace FasterState;

void Faster::HitReactionFinalize([[maybe_unused]] const States& pre) {



}

void Faster::HitReactionInit([[maybe_unused]] const States& pre)
{
	
	SetAnimation("Damage", false);

}

void Faster::HitReactionUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	SetPosition(GetPosition() + (GetKnockBackVelocity() + GetRepulsiveForce()) *
		LWP::Info::GetDeltaTimeF());
	//ノックバック減衰
	SetKnockBackVelocity(GetKnockBackVelocity() * HitReactionParameter::decay);

	//ノックバックが終了した時
	if (not animation_.GetPlaying()) {

		//直前の状態を再開
		state_.request = preState_;
		return;

	}

}

