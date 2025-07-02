#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// ふり降ろし攻撃の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::SwingDownAttackFinalize([[maybe_unused]] const States& pre) {

}

void Ogre::SwingDownAttackInit([[maybe_unused]] const States& pre)
{

	SetAnimation("LightAttack2", false);
	//もしノックバックしたら待機に戻る
	preState_ = States::kIdle;

	//AABBで攻撃判定実装


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
