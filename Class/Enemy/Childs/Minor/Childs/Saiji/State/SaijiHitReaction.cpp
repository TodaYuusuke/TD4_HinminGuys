#include "../../../../../../Player/Player.h"
#include "../Saiji.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace SaijiState;

void Saiji::HitReactionFinalize(const States& pre) {



}

void Saiji::HitReactionInit(const States& pre)
{
	
	SetAnimation("Damage", false);

}

void Saiji::HitReactionUpdate(std::optional<States>& req, const States& pre)
{

	SetPosition(GetPosition() + (GetKnockBackVelocity() + GetRepulsiveForce()) *
		LWP::Info::GetDeltaTime());
	//ノックバック減衰
	SetKnockBackVelocity(GetKnockBackVelocity() * HitReactionParameter::decay);

	//ノックバックが終了した時
	if (not animation_.GetPlaying()) {

		//直前の状態を再開
		state_.request = preState_;
		return;

	}

}

