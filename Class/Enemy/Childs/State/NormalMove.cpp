#include "NormalMove.h"
#include "NormalAttack.h"
#include "NormalIdle.h"
#include "WaitingForAttack.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

float NormalMove::attackDist_ = 0.5f;
int32_t NormalMove::runTime_ = 120;

void NormalMove::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Run", true);
	//時間セット
	countRunTime_ = runTime_;


}

void NormalMove::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {

		//カウントダウン
		if (countRunTime_ > 0) {
			countRunTime_--;
		}

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

		//0になったら行動を切り替える
		if (countRunTime_ <= 0) {

			//待機状態に移行
			enemy_->SubAttackCount();
			enemy_->SetState(new NormalIdle());
			return;

		}

		//移動
		velocity_ = enemy_->GetPlayerPosition() - enemy_->GetPosition();
		//y軸の移動ベクトルを消す
		velocity_.y = 0.0f;

		velocity_ = velocity_.Normalize() * LWP::Info::GetDeltaTime();

		enemy_->SetPosition(enemy_->GetPosition() + velocity_ + (enemy_->GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

		//プレイヤーの向きに回転
		enemy_->RotateTowardsPlayer();

	}

}

void NormalMove::DebugGUI()
{

	if (ImGui::TreeNode("NormalMove")) {
		ImGui::DragFloat("attackDist", &attackDist_, 0.1f);
		ImGui::DragInt("runTime", &runTime_, 0.2f);
		ImGui::TreePop();
	}

}
