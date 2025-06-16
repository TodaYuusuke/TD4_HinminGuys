#include "../../../../Player/Player.h"
#include "../Normal.h"
#include "../../../EnemyManager.h"

using namespace LWP::Math;

void Normal::HitReactionFinalize(const States& pre) {



}

void Normal::HitReactionInit(const States& pre)
{
	
	SetAnimation("Damage", false);

}

void Normal::HitReactionUpdate(std::optional<States>& req, const States& pre)
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

