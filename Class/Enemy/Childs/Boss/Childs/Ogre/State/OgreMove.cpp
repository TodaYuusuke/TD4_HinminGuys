#include "../../../../../../Player/Player.h"
#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

/// <summary>
/// 移動の実装
/// </summary>

using namespace LWP::Math;
using namespace OgreState;

void Ogre::MoveFinalize([[maybe_unused]] const States& pre) {
	//スピードをリセット
	currentMotionSpeed_ = 1.0f;
	//待機ステートの待機時間セット
	stateParameter_.idleParameter.countStandTime = stateParameter_.idleParameter.standTime;
	//デフォの移動速度セット
	parameter_.speed = 1.0f;

}

void Ogre::MoveInit([[maybe_unused]] const States& pre)
{

	SetAnimation("Run", true);
	preState_ = States::kMove;
	//移動速度をセット
	parameter_.speed = stateParameter_.moveParameter.defaultSpeed;

}

void Ogre::MoveUpdate([[maybe_unused]] std::optional<States>& req, [[maybe_unused]] const States& pre)
{

	//プレイヤーが存在する場合
	if (player_) {

		//カウントダウン
		if (stateParameter_.moveParameter.countRunTime > 0) {
			stateParameter_.moveParameter.countRunTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

		//プレイヤーとの距離が近い場合
		if (Vector3::Distance(GetPlayerPosition(), GetPosition()) < stateParameter_.moveParameter.attackDist) {

			//弱攻撃カウントが規定数に達した場合、中攻撃もしくは強攻撃に移行
			if (stateParameter_.moveParameter.lightAttackCount >= stateParameter_.moveParameter.lightTransitionCount) {

				//カウントリセット
				stateParameter_.moveParameter.lightAttackCount = 0;

				//抽選して、0の場合中攻撃
				if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
					//中攻撃カウント増加
					stateParameter_.moveParameter.mediumAttackCount++;
					//落下攻撃に移行する
					state_.request = States::kFallingThrust;
					return;
				}
				//強攻撃
				else {

					//強攻撃の2つの中で抽選し、0なら連続突撃
					if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
						state_.request = States::kAssaultSlash;
						return;
					}
					//1なら四連撃
					else {
						state_.request = States::kQuadrupleAttack;
						return;
					}

				}

			}
			//中攻撃カウントが規定数に達した場合、強攻撃に移行
			else if (stateParameter_.moveParameter.mediumAttackCount >= stateParameter_.moveParameter.mediumTransitionCount) {

				//カウントリセット
				stateParameter_.moveParameter.mediumAttackCount = 0;

				//強攻撃の2つの中で抽選し、0なら連続突撃
				if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
					state_.request = States::kAssaultSlash;
					return;
				}
				//1なら四連撃
				else {
					state_.request = States::kQuadrupleAttack;
					return;
				}

			}
			//どちらのカウントも達していない場合、2つの弱攻撃のどちらかに移行
			else {

				//弱攻撃カウント増加
				stateParameter_.moveParameter.lightAttackCount++;

				//弱攻撃の2つの中で抽選し、0ならふり降ろし
				if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
					state_.request = States::kSwingDownAttack;
					return;
				}
				//1なら回転切り
				else {
					state_.request = States::kRotatingSlash;
					return;
				}

			}

			return;
		}

		//0になったら行動を切り替える
		if (stateParameter_.moveParameter.countRunTime <= 0) {

			//待機状態に移行
			isAttackPhase_ = false;
			state_.request = States::kIdle;
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

		//プレイヤーの向きに回転
		RotateTowardsPlayer();

	}

}

