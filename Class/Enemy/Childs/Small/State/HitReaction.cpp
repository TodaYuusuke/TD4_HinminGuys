#include "../../../../Player/Player.h"
#include "../Small.h"
#include "../../../EnemyManager.h"

using namespace LWP::Math;

void Small::HitReactionFinalize(const States& pre) {



}

void Small::HitReactionInit(const States& pre)
{
	
	SetAnimation("Damage", false);

}

void Small::HitReactionUpdate(std::optional<States>& req, const States& pre)
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

