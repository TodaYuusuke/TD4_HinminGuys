#include "NormalMove.h"
#include "NormalAttack.h"
#include "NormalIdle.h"
#include "WaitingForAttack.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void NormalMove::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Run", true);
	//走り時間
	runTime_ = 60;
	//追加走り時間
	overRunTime_ = 90;
	//プレイヤーとの距離が近ければ距離を取り、遠ければ近づく
	isApproach_ = true;
	if (Vector3::Distance(enemy_->GetPlayerPosition(), enemy_->GetPosition()) < approachDist_) {
		isApproach_ = false;
	}


}

void NormalMove::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {

		//カウントダウン
		if (runTime_ > 0) {
			runTime_--;
		}

		//0になったら行動を切り替える
		if (runTime_ <= 0) {

			//プレイヤーとの距離が近く、誰も攻撃していなかったら攻撃
			if (Vector3::Distance(enemy_->GetPlayerPosition(), enemy_->GetPosition()) < attackDist_) {

				//誰も攻撃していない状態で、攻撃待機中の敵もいない場合、攻撃にそのまま移行
				if (not enemy_->GetIsAttack() and 
					WaitingForAttack::GetAttackCount() == WaitingForAttack::GetNextAttackCount()) {
					//攻撃状態に移行
					enemy_->SetState(new NormalAttack());
				}
				else {
					//攻撃待機状態に移行
					enemy_->SetState(new WaitingForAttack());
				}

				return;
			}
			//一定以上離れている場合
			else {

				//追加で移動時間をカウント
				if (overRunTime_ > 0) {
					overRunTime_--;
				}

				//追加移動時間を超えた場合
				if (overRunTime_ <= 0) {

					//プレイヤーとの距離が近く、誰も攻撃していなかったら攻撃
					if (Vector3::Distance(enemy_->GetPlayerPosition(), enemy_->GetPosition()) < attackDist_) {

						//誰も攻撃していない状態で、攻撃待機中の敵もいない場合、攻撃にそのまま移行
						if (not enemy_->GetIsAttack() and
							WaitingForAttack::GetAttackCount() == WaitingForAttack::GetNextAttackCount()) {
							//攻撃状態に移行
							enemy_->SetState(new NormalAttack());
						}
						else {
							//攻撃待機状態に移行
							enemy_->SetState(new WaitingForAttack());
						}

						return;
					}
					//そうでないなら待機状態に戻す
					else {
						//待機状態に移行
						enemy_->SetState(new NormalIdle());
						return;
					}

				}

			}

		}

		//移動
		velocity_ = enemy_->GetPlayerPosition() - enemy_->GetPosition();
		//y軸の移動ベクトルを消す
		velocity_.y = 0.0f;
		//敵の向きをプレイヤーに向かせるため、ここで向きを保存
		Vector3 to = velocity_.Normalize();
		velocity_ = velocity_.Normalize() * LWP::Info::GetDeltaTime();
		//近づかない場合、通常移動時間中は移動方向を逆にする
		if (not isApproach_ and runTime_ > 0) {
			velocity_ *= -1.0f;
		}
		enemy_->SetPosition(enemy_->GetPosition() + velocity_ + (enemy_->GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

		//プレイヤーの向きに回転
		enemy_->RotateTowardsPlayer();

	}

}
