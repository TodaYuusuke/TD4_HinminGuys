#include "NormalAttack.h"
#include "NormalIdle.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"
#include "../../../GameMask.h"

using namespace GameMask;

NormalAttack::NormalAttack()
	: capsule_(collider_.SetBroadShape(LWP::Object::Collider::Capsule()))
{
}

void NormalAttack::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("LightAttack2", false);
	//攻撃判定オン
	enemy_->BeginAttack();

	// 体の判定生成
	collider_.SetFollow(&enemy_->GetSwordModel().worldTF);
	collider_.isActive = true;
	// 自機の所属しているマスクを設定
	collider_.mask.SetBelongFrag(GetAttack());
	// 当たり判定をとる対象のマスクを設定
	collider_.mask.SetHitFrag(GetPlayer());
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		enemy_->GetPlayer()->ChangeHPGauge(enemy_->GetAttackParameter().attackValue);
		};

}

void NormalAttack::Update()
{

	//攻撃が終了した時
	if (not enemy_->GetAnimation()->GetPlaying()) {

		//攻撃判定オフ

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
