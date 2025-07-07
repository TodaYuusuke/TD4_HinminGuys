#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 落下攻撃の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::FallingThrustFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	isAttack_ = false;
	isAttackPhase_ = false;
	//中攻撃終了時の抽選処理
	EndMediumAttack();
}

void Ogre::FallingThrustInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Damage", false);
	//もしノックバックしたら待機に戻る
	preState_ = States::kIdle;
	//球で実装

}

void Ogre::FallingThrustUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//攻撃が終了した時
	if (not animation_.GetPlaying()) {
		isAttack_ = false;
		state_.request = States::kIdle;
		return;
	}

}
