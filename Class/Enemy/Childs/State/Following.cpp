#include "Following.h"
#include "NormalIdle.h"
#include "../../IEnemy.h"

float Following::idleDist_ = 4.0f;

Following::Following()
{
}

Following::~Following()
{
}

void Following::Initialize(IEnemy* enemy)
{
	
	enemy_ = enemy;
	enemy_->SetAnimation("Run", true);

}

void Following::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {

		//待機状態に移行する距離になるまでプレイヤーに向かう
		if (enemy_->GetDistFromPlayer() < idleDist_) {
			enemy_->SetState(new NormalIdle());
			return;
		}

		Vector3 result{};

		//移動
		result = enemy_->GetPlayerPosition() - enemy_->GetPosition();
		//y軸の移動ベクトルを消す
		result.y = 0.0f;
		result = result.Normalize() * LWP::Info::GetDeltaTime();
		
		enemy_->SetPosition(enemy_->GetPosition() + result + (enemy_->GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

		//プレイヤーの向きに回転
		enemy_->RotateTowardsPlayer();

	}

}

void Following::DebugGUI()
{

	if (ImGui::TreeNode("Following")) {
		ImGui::DragFloat("idleDist", &idleDist_, 0.1f);
		ImGui::TreePop();
	}

}
