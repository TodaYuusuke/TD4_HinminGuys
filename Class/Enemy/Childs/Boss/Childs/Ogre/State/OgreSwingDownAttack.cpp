#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OgreState;

void Ogre::SwingDownAttackFinalize([[maybe_unused]] const States& pre) {

}

void Ogre::SwingDownAttackInit([[maybe_unused]] const States& pre)
{

	SetAnimation("LightAttack2", false);

}

void Ogre::SwingDownAttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//攻撃が終了した時
	if (not animation_.GetPlaying()) {
		isAttack_ = false;
		state_.request = States::kIdle;
		return;
	}

}
