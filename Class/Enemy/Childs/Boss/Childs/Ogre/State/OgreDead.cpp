#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

using namespace OgreState;

void Ogre::DeadFinalize(const States& pre)
{
}

void Ogre::DeadInit(const States& pre)
{

	animation_.Play("Idle", 0.2f)
		.Loop(false);

}

void Ogre::DeadUpdate(std::optional<States>& req, const States& pre)
{

	//アニメーションが終了した時
	if (not animation_.GetPlaying()) {
		//死亡フラグを立てる
		isDead_ = true;

	}

}
