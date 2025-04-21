#include "NormalEnemyMove.h"
#include "NormalEnemyAttack.h"
#include "NormalEnemyIdle.h"
#include "../../../../Player/Player.h"
#include "../../../IEnemy.h"

void NormalEnemyMove::Initialize(IEnemy* enemy)
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

void NormalEnemyMove::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {

		//カウントダウン
		if (runTime_ > 0) {
			runTime_--;
		}

		//0になったら行動を切り替える
		if (runTime_ <= 0) {

			//プレイヤーとの距離が近い場合
			if (Vector3::Distance(enemy_->GetPlayerPosition(), enemy_->GetPosition()) < attackDist_) {
				//攻撃状態に移行
				enemy_->SetState(std::make_unique<NormalEnemyAttack>());
			}
			//一定以上離れている場合
			else {

				//追加で移動時間をカウント
				if (overRunTime_ > 0) {
					overRunTime_--;
				}

				//追加移動時間を超えた場合
				if (overRunTime_ <= 0) {

					//プレイヤーとの距離が近いなら攻撃
					if (Vector3::Distance(enemy_->GetPlayerPosition(), enemy_->GetPosition()) < attackDist_) {
						//攻撃状態に移行
						enemy_->SetState(std::make_unique<NormalEnemyAttack>());
					}
					//そうでないなら待機状態に戻す
					else {
						//待機状態に移行
						enemy_->SetState(std::make_unique<NormalEnemyIdle>());
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

		//プレイヤーの方向を向く
		if (velocity_.Length() != 0.0f) {
			//敵の基本姿勢
			Vector3 from = { 0.0f,0.0f,1.0f };

			// 回転軸をクロス積から求める
			Vector3 axis = Vector3::Cross(from, to);
			// 内積
			float dot = Vector3::Dot(from, to);
			// 完全に平行な場合、単位クォータニオンを返す
			if (dot > 0.9999f) {

				//行きたい方向のQuaternionの作成
				enemy_->SetRotation(Quaternion{ 0.0f,0.0f,0.0f,1.0f });

			}
			else {

				//逆向きのベクトルだった場合、垂直なベクトルを一つ選ぶ
				if (dot <= -1.0f) {

					if (from.x != 0.0f || from.y != 0.0f) {

						axis = { from.y, -from.x,0.0f };
						axis = axis.Normalize();
					}
					else if (from.x != 0.0f || from.z != 0.0f) {

						axis = { 0.0f, -from.z, from.x };
						axis = axis.Normalize();

					}

				}
				else {
					axis = Vector3::Cross(from, to).Normalize();
				}

				// θを求める
				float theta = std::acos(Vector3::Dot(from, to) / (from.Length() * to.Length()));

				//行きたい方向のQuaternionの作成
				enemy_->SetRotation(Quaternion::CreateFromAxisAngle(axis, theta));

			}

		}

	}

}
