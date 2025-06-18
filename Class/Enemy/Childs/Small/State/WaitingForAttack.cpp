#include "../Small.h"
#include "../../../EnemyManager.h"

using namespace LWP::Math;

void Small::WaitingForAttackFinalize(const States& pre)
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

void Small::WaitingForAttackInit(const States& pre)
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

void Small::WaitingForAttackUpdate(std::optional<States>& req, const States& pre)
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

		//移動
		Vector3 dist = GetPlayerPosition() - GetPosition();

		//y軸の移動ベクトルを消す
		dist.y = 0.0f;
		//距離
		float length = dist.Length();
		//正規化
		dist = dist.Normalize();

		//方向ベクトル
		Vector3 direction = dist * -1.0f;

		//角度を求める
		float theta = std::acosf(Vector3::Dot({ 1.0f,0.0f,0.0f }, direction));
		//外積を求めて正負判定(direction.zの値がそのまま正負になる)
		if (direction.z < 0.0f) {
			theta *= -1.0f;
		}

		//円周を沿うような移動ベクトルにする
		Vector3 result{};

		result.x = -sinf(theta);
		result.z = cosf(theta);

		//右回りならベクトルを逆にする
		if (stateParameter_.waitingForAttackParameter.isClockwise) {
			result *= -1.0f;
		}

		SetPosition(GetPosition() + result * LWP::Info::GetDeltaTime() + (GetRepulsiveForce() * LWP::Info::GetDeltaTime()));
		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}
