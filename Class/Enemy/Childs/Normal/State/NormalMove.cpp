#include "NormalMove.h"
#include "NormalAttack.h"
#include "NormalIdle.h"
#include "WaitingForAttack.h"
#include "../../../../Player/Player.h"
#include "../Normal.h"
#include "../../../EnemyManager.h"

float NormalMove::attackDist_ = 0.5f;
int32_t NormalMove::runTime_ = 120;

NormalMove::NormalMove(Normal* enemy)
{

	enemy_ = enemy;
	enemy_->SetAnimation("Run", true);
	stateType_ = States::kNormalMove;

}

void NormalMove::Initialize()
{
	
	//時間セット
	enemy_->GetStateParameter().moveParameter.countRunTime = runTime_;


}

void NormalMove::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayerPtr()) {

		//カウントダウン
		if (enemy_->GetStateParameter().moveParameter.countRunTime > 0) {
			enemy_->GetStateParameter().moveParameter.countRunTime--;
		}

		//プレイヤーとの距離が近く、誰も攻撃していなかったら攻撃
		if (Vector3::Distance(enemy_->GetPlayerPosition(), enemy_->GetPosition()) < attackDist_) {

			//誰も攻撃していない状態で、攻撃待機中の敵もいない場合、攻撃にそのまま移行
			if (not enemy_->GetManagerPtr()->IsAnyAttack() and
				WaitingForAttack::GetAttackCount() == WaitingForAttack::GetNextAttackCount()) {
				//攻撃状態に移行
				enemy_->SetState(States::kNormalAttack);
			}
			else {
				//攻撃待機状態に移行
				enemy_->SetState(States::kWaitingForAttack);
			}

			return;
		}

		//0になったら行動を切り替える
		if (enemy_->GetStateParameter().moveParameter.countRunTime <= 0) {

			//待機状態に移行
			enemy_->SetIsAttackPhase(false);
			enemy_->SetState(States::kNormalIdle);
			return;

		}

		//移動
		enemy_->GetStateParameter().moveParameter.velocity = enemy_->GetPlayerPosition() - enemy_->GetPosition();
		//y軸の移動ベクトルを消す
		enemy_->GetStateParameter().moveParameter.velocity.y = 0.0f;

		enemy_->GetStateParameter().moveParameter.velocity =
			enemy_->GetStateParameter().moveParameter.velocity.Normalize() * LWP::Info::GetDeltaTime();

		enemy_->SetPosition(enemy_->GetPosition() + enemy_->GetStateParameter().moveParameter.velocity +
			(enemy_->GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

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
