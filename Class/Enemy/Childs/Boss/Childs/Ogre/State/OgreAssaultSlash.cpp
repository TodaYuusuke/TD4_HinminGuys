#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OgreState;

void Ogre::AssaultSlashFinalize([[maybe_unused]] const States& pre) {

}

void Ogre::AssaultSlashInit([[maybe_unused]] const States& pre)
{

	SetAnimation("LightAttack1", false);

}

void Ogre::AssaultSlashUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//攻撃が終了した時
	if (not animation_.GetPlaying()) {
		isAttack_ = false;
		state_.request = States::kIdle;
		return;
	}

}
