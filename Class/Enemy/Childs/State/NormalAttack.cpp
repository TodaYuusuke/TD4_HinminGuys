#include "NormalAttack.h"
#include "NormalIdle.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"
#include "../../../GameMask.h"

using namespace GameMask;

NormalAttack::NormalAttack()
{
}

void NormalAttack::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("LightAttack2", false, 0.1f);
	enemy_->GetSwordCollider().isActive = false;
	enemy_->BeginAttack();
	//パリィエフェクト開始
	enemy_->StartParryEffect();

}

void NormalAttack::Update()
{

	//パリィエフェクト中ならアニメーションをゆっくりにして判定オフ
	if (enemy_->GetIsStartParryEffect()) {
		enemy_->GetAnimation()->GetPlayBackSpeed() = 0.1f;
		enemy_->GetSwordCollider().isActive = false;
	}
	//パリィエフェクトが終わったら通常スピードで判定をオンにする
	else {
		enemy_->GetAnimation()->GetPlayBackSpeed() = 1.0f;
		enemy_->GetSwordCollider().isActive = true;
	}

	//攻撃が終了した時
	if (not enemy_->GetAnimation()->GetPlaying()) {

		//攻撃判定オフ
		enemy_->GetSwordCollider().isActive = false;
		//待機状態に移行
		enemy_->EndAttack();
		enemy_->SubAttackCount();
		enemy_->SetState(new NormalIdle());
		return;

	}

}

void NormalAttack::DebugGUI()
{

	if (ImGui::TreeNode("NormalAttack")) {
		ImGui::TreePop();
	}

}
