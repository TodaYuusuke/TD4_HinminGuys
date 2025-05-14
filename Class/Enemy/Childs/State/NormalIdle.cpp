#include "NormalIdle.h"
#include "NormalMove.h"
#include "Spacing.h"
#include "Following.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

int32_t NormalIdle::standTime_ = 60;
float NormalIdle::followingDist_ = 6.0f;

void NormalIdle::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Idle", true);
	//待機時間
	countStandTime_ = standTime_;
}

void NormalIdle::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {
		//カウントダウン
		if (countStandTime_ > 0) {
			countStandTime_--;
		}

		//プレイヤーから一定以上の距離離れたら追従モーションに移行
		if (enemy_->GetDistFromPlayer() > followingDist_) {
			enemy_->SetState(new Following());
			return;
		}

		//0になったら状態切り替え
		if (countStandTime_ <= 0) {

			//間合い取り状態に移行
			enemy_->SetState(new Spacing());
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
