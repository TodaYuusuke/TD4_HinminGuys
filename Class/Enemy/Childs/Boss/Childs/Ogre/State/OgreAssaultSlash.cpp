#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 連続突撃の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::AssaultSlashFinalize([[maybe_unused]] const States& pre) {

}

void Ogre::AssaultSlashInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Idle", true);
	//もしノックバックしたら待機に戻る
	preState_ = States::kIdle;

	//AABB実装

}

void Ogre::AssaultSlashUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	

}
