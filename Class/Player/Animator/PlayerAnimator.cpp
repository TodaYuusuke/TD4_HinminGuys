#include "PlayerAnimator.h"
#include "../Player.h"

void PlayerAnimator::Initialize(LWP::Resource::Animation* animation) {
	animation_ = animation;

	// コマンド作成
	CreateCommand();
}

void PlayerAnimator::Update(Player& player) {
	// 入力されたコマンドを確認
	animations_ = HandleAnimation(player);

	// set Command
	for (IAnimation* cmd : animations_) {
		cmd->Exec(player);
	}
}

void PlayerAnimator::DebugGUI() {
	if (ImGui::TreeNode("Command")) {
		// 登録されているコマンド
		if (ImGui::TreeNode("AllView")) {

			ImGui::TreePop();
		}
		// 登録をし直す
		if (ImGui::TreeNode("Reset")) {

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void PlayerAnimator::CreateCommand() {
	AssignMoveCommand();
	AssignAttackCommand();
	AssignParryCommand();
	AssignEvasionCommand();
	AssignSheathCommand();
}

std::vector<IAnimation*> PlayerAnimator::HandleAnimation(Player& player) {
	std::vector<IAnimation*> result;

	// パリィ
	if (player.GetSystemManager()->GetParrySystem()->GetIsActive()) {
		result.push_back(parryAnim_);
	}
	// 回避
	if (player.GetSystemManager()->GetEvasionSystem()->GetIsActive()) {
		result.push_back(evasionAnim_);
	}
	// 鞘
	if (player.GetSystemManager()->GetSheathSystem()->GetIsActive()) { 
		result.push_back(sheathAnim_);
	}
	// 移動
	if (!player.GetSystemManager()->GetParrySystem()->GetIsActive() && !player.GetSystemManager()->GetAttackSystem()->GetIsActive() && !player.GetSystemManager()->GetEvasionSystem()->GetIsActive() && !player.GetSystemManager()->GetSheathSystem()->GetIsActive()) {
		result.push_back(moveAnim_);
	}

	return result;
}

void PlayerAnimator::AssignMoveCommand() {
	IAnimation* animation = new MoveAnim();
	this->moveAnim_ = animation;
}

void PlayerAnimator::AssignAttackCommand() {
	IAnimation* animation = new AttackAnim();
	this->attackAnim_ = animation;
}

void PlayerAnimator::AssignParryCommand() {
	IAnimation* animation = new ParryAnim();
	this->parryAnim_ = animation;
}

void PlayerAnimator::AssignEvasionCommand() {
	IAnimation* animation = new EvasionAnim();
	this->evasionAnim_ = animation;
}

void PlayerAnimator::AssignSheathCommand() {
	IAnimation* animation = new SheathAnim();
	this->sheathAnim_ = animation;
}