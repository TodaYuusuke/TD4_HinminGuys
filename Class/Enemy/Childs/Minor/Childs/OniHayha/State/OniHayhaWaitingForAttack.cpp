#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace OniHayhaState;

void OniHayha::WaitingForAttackFinalize([[maybe_unused]] const States& pre)
{

	//消えたときのIDが一致している場合
	if (stateParameter_.waitingForAttackParameter.attackID == WaitingForAttackParameter::nextAttackCount) {
		//次攻撃する番号を上昇させる
		WaitingForAttackParameter::nextAttackCount++;
	}
	//違う場合
	else {
		//攻撃の順番を決める数字を減少させる
		WaitingForAttackParameter::attackCount--;
	}

}

void OniHayha::WaitingForAttackInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Run", true);

	//現在の攻撃カウントから順番を決める
	stateParameter_.waitingForAttackParameter.attackID = WaitingForAttackParameter::attackCount;
	//攻撃の順番を決める数字を上昇させる
	WaitingForAttackParameter::attackCount++;

	//ランダムな数字を利用して右回りかどうかを決める
	if (LWP::Utility::GenerateRandamNum(0, 1) == 0) {
		stateParameter_.waitingForAttackParameter.isClockwise = true;
	}

	preState_ = States::kWaitingForAttack;

}

void OniHayha::WaitingForAttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//
	//一部間合いを取るクラスのコピペをしているので後々修正する
	//

	//誰も攻撃しておらず、順番が回ってきたら攻撃に移行
	if (not enemyManager_->IsAnyAttack() and 
		stateParameter_.waitingForAttackParameter.attackID == WaitingForAttackParameter::nextAttackCount) {
		//攻撃状態に移行
		state_.request = States::kAttack;
		return;
	}

	//プレイヤーが存在する場合
	if (player_) {

		

	}

}
