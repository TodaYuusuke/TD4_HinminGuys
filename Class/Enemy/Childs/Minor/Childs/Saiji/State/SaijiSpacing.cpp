#include "../Saiji.h"
#include "../../../../../EnemyManager.h"

using namespace LWP::Math;
using namespace SaijiState;

void Saiji::SpacingFinalize(const States& pre)
{

	//移動ステートの時間セット
	stateParameter_.moveParameter.countRunTime = MoveParameter::runTime;
	stateParameter_.moveParameter.speed = MoveParameter::defaultSpeed;
	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = IdleParameter::standTime;

}

void Saiji::SpacingInit(const States& pre)
{
	
	SetAnimation("Run", true, 0.3f);
	preState_ = States::kSpacing;

}

void Saiji::SpacingUpdate(std::optional<States>& req, const States& pre)
{

	//カウントダウン
	if (stateParameter_.spacingParameter.countSpacingTime > 0) {
		stateParameter_.spacingParameter.countSpacingTime -= 1.0f * LWP::Info::GetDeltaTimeF();
	}

	//0になったら行動変化
	if (stateParameter_.spacingParameter.countSpacingTime <= 0) {

		//攻撃人数が3人未満且つ敵の中で距離の近さが3位以内の時
		if (enemyManager_->GetAttackPhaseCount() < IEnemy::GetMaxAttackCount() and
			GetClosenessCount() < IEnemy::GetMaxAttackCount()) {
			//接近状態に移行
			SetIsAttackPhase(true);
			state_.request = States::kMove;
			return;
		}
		//そうでない場合
		else {
			//待機状態に戻る
			state_.request = States::kIdle;
			return;
		}

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
		if (stateParameter_.spacingParameter.isClockwise) {
			result *= -1.0f;
		}

		//プレイヤーとの間合いをあらかじめ決めておき、その範囲内に入ったら押し出しベクトルを加算するようにする
		if (length < SpacingParameter::spaceDist && length > 0.0001f) {
			AddRepulsiveForce(dist.Normalize() * 
				-((SpacingParameter::spaceDist - length) * 2.0f / SpacingParameter::spaceDist));
		}

		result = result + dist.Normalize();

		result = result.Normalize();

		SetPosition(GetPosition() + result * LWP::Info::GetDeltaTime() + (GetRepulsiveForce() * LWP::Info::GetDeltaTime()));
		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}

