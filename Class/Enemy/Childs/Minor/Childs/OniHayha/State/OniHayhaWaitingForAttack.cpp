#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OniHayhaState;

void OniHayha::WaitingForAttackFinalize([[maybe_unused]] const States& pre)
{

	//消えたときのIDが一致している場合
	if (stateParameter_.waitingForAttackParameter.attackID == enemyManager_->longNextAttackID) {
		//次攻撃する番号を上昇させる
		enemyManager_->longNextAttackID++;
	}
	//違う場合
	else {
		//攻撃の順番を決める数字を減少させる
		enemyManager_->longAssignAttackID--;
	}

	laserModel_.isActive = false;

}

void OniHayha::WaitingForAttackInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Idle", true);

	//現在の攻撃カウントから順番を決める
	stateParameter_.waitingForAttackParameter.attackID = enemyManager_->longAssignAttackID;
	//攻撃の順番を決める数字を上昇させる
	enemyManager_->longAssignAttackID++;

	laserModel_.isActive = true;

}

void OniHayha::WaitingForAttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{


	//誰も攻撃しておらず、順番が回ってきたら攻撃に移行
	if (not enemyManager_->IsAnyAttack() and 
		stateParameter_.waitingForAttackParameter.attackID == enemyManager_->longNextAttackID) {
		//狙い撃ちの待機時間セット
		stateParameter_.aimingParameter.countAimingTime = stateParameter_.aimingParameter.aimingTime;
		//狙い状態に移行
		state_.request = States::kAiming;
		return;
	}

	if (player_) {
		//プレイヤーの向きに回転
		RotateTowardsPlayer();
	}

}
