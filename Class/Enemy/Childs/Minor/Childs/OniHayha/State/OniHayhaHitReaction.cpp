#include "../../../../../../Player/Player.h"
#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OniHayhaState;

void OniHayha::HitReactionFinalize([[maybe_unused]] const States& pre) {



}

void OniHayha::HitReactionInit([[maybe_unused]] const States& pre)
{
	
	animation_.Play("Damage", 0.2f)
		.Loop(false);

}

void OniHayha::HitReactionUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	SetPosition(GetPosition() + (GetKnockBackVelocity() + GetRepulsiveForce()) *
		LWP::Info::GetDeltaTimeF());
	//ノックバック減衰
	SetKnockBackVelocity(GetKnockBackVelocity() * stateParameter_.hitReactionParameter.decay);

	//ノックバックが終了した時
	if (not animation_.GetPlaying()) {

		//直前の状態を再開
		state_.request = preState_;
		return;

	}

}

