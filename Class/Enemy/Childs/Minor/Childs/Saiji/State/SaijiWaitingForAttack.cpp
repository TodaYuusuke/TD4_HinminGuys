#include "../Saiji.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace SaijiState;

void Saiji::WaitingForAttackFinalize([[maybe_unused]] const States& pre)
{

	//消えたときのIDが一致している場合
	if (stateParameter_.waitingForAttackParameter.attackID == enemyManager_->shortNextAttackID) {
		//次攻撃する番号を上昇させる
		enemyManager_->shortNextAttackID++;
	}
	//違う場合
	else {
		//攻撃の順番を決める数字を減少させる
		enemyManager_->shortAssignAttackID--;
	}

	//デフォの移動速度セット
	parameter_.speed = 1.0f;

}

void Saiji::WaitingForAttackInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Run", true, 0.3f);

	//現在の攻撃カウントから順番を決める
	stateParameter_.waitingForAttackParameter.attackID = enemyManager_->shortAssignAttackID;
	//攻撃の順番を決める数字を上昇させる
	enemyManager_->shortAssignAttackID++;

	//ランダムな数字を利用して右回りかどうかを決める
	if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
		stateParameter_.waitingForAttackParameter.isClockwise = true;
	}

	preState_ = States::kWaitingForAttack;

	//移動速度をセット
	parameter_.speed = stateParameter_.moveParameter.defaultSpeed;

}

void Saiji::WaitingForAttackUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//誰も攻撃しておらず、順番が回ってきたら走る
		if (not enemyManager_->IsAnyAttack() and
			stateParameter_.waitingForAttackParameter.attackID == enemyManager_->shortNextAttackID) {
			//速度を戻す
			animation_.GetPlayBackSpeed() = 1.0f;

			//攻撃範囲に入ったら
			if (Vector3::Distance(GetPlayerPosition(), GetPosition()) < stateParameter_.moveParameter.attackDist) {
				//攻撃状態に移行
				state_.request = States::kAttack;
				return;
			}
			
			//移動
			stateParameter_.moveParameter.direction = GetPlayerPosition() - GetPosition();
			//y軸の移動ベクトルを消す
			stateParameter_.moveParameter.direction.y = 0.0f;

			stateParameter_.moveParameter.direction =
				stateParameter_.moveParameter.direction.Normalize() *
				parameter_.speed * LWP::Info::GetDeltaTimeF();

			SetPosition(GetPosition() + stateParameter_.moveParameter.direction +
				(GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));

		}
		else {

			//移動
			Vector3 dist = GetPlayerPosition() - GetPosition();

			//y軸の移動ベクトルを消す
			dist.y = 0.0f;
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

			SetPosition(GetPosition() + result * LWP::Info::GetDeltaTimeF() * 1.0f
				+ (GetRepulsiveForce() * LWP::Info::GetDeltaTimeF()));

		}

		
		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}
