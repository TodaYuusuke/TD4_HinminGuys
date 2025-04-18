#include "NormalEnemyMove.h"
#include "NormalEnemyAttack.h"
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

				//追加移動時間も超えたら強制的に攻撃へ移行
				if (overRunTime_ <= 0) {
					//攻撃状態に移行
					enemy_->SetState(std::make_unique<NormalEnemyAttack>());
				}

			}

		}

		//移動
		velocity_ = enemy_->GetPlayerPosition() - enemy_->GetPosition();
		//y軸の移動ベクトルを消す
		velocity_.y = 0.0f;
		velocity_ = velocity_.Normalize() * LWP::Info::GetDeltaTime();
		//近づかない場合、通常移動時間中は移動方向を逆にする
		if (not isApproach_ and runTime_ > 0) {
			velocity_ *= -1.0f;
		}
		enemy_->SetPosition(enemy_->GetPosition() + velocity_);

		//プレイヤーの方向を向く
		if (velocity_.Length() != 0.0f) {

			//行きたい方向のQuaternionの作成
			enemy_->SetRotation(Quaternion::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, velocity_.Normalize()));

		}

	}

}
