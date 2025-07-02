#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// ノックバックの実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::HitReactionFinalize([[maybe_unused]] const States& pre) {



}

void Ogre::HitReactionInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Damage", false);

}

void Ogre::HitReactionUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
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

