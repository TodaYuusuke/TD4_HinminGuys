#include "NormalIdle.h"
#include "NormalMove.h"
#include "Spacing.h"
#include "Following.h"
#include "../../../../Player/Player.h"
#include "../Normal.h"
#include "../../../EnemyManager.h"

int32_t NormalIdle::standTime_ = 60;
float NormalIdle::followingDist_ = 6.0f;

NormalIdle::NormalIdle(Normal* enemy)
{

	enemy_ = enemy;
	enemy_->SetAnimation("Idle", true);
	stateType_ = States::kNormalIdle;

}

void NormalIdle::Initialize()
{
	
	//待機時間
	enemy_->GetStateParameter().idleParameter.countStandTime = standTime_;
}

void NormalIdle::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayerPtr()) {
		//カウントダウン
		if (enemy_->GetStateParameter().idleParameter.countStandTime > 0) {
			enemy_->GetStateParameter().idleParameter.countStandTime--;
		}

		//プレイヤーから一定以上の距離離れたら追従モーションに移行
		if (enemy_->GetDistFromPlayer() > followingDist_) {
			enemy_->SetState(States::kFollowing);
			return;
		}

		//0になったら状態切り替え
		if (enemy_->GetStateParameter().idleParameter.countStandTime <= 0) {

			//間合い取り状態に移行
			enemy_->SetState(States::kSpacing);
			return;
		}

	}

}

void NormalIdle::DebugGUI()
{

	if (ImGui::TreeNode("NormalIdle")) {
		ImGui::DragFloat("followingDist", &followingDist_, 0.1f);
		ImGui::TreePop();
	}

}
