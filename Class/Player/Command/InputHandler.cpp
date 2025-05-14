#include "InputHandler.h"
#include "InputConfig.h"

using namespace LWP::Input;
using namespace InputConfig;

void InputHandler::Initialize() {
	// コマンド作成
	CreateCommand();
}

void InputHandler::Update(Player& player) {
	// 入力されたコマンドを確認
	commands_ = HandleInput();

	// set Command
	for (ICommand* cmd : commands_) {
		cmd->Exec(player);
	}
}

void InputHandler::DebugGUI() {
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

void InputHandler::CreateCommand() {
	AssignNormalAttackCommand();
	AssignParryCommand();
	AssignLockOnCommand();
	AssignEvasionCommand();
}

std::vector<ICommand*> InputHandler::HandleInput() {
	std::vector<ICommand*> result;

	// 通常攻撃コマンド
	if (Keyboard::GetTrigger(Command::Key::NormalAttack) || Pad::GetTrigger(Command::GamePad::NormalAttack)) {
		result.push_back(pressNormalAttackCommand_);
	}
	// パリィコマンド
	if (Keyboard::GetTrigger(Command::Key::Parry) || Pad::GetTrigger(Command::GamePad::Parry)) {
		result.push_back(pressParryCommand_);
	}
	// ロックオンコマンド
	if (Keyboard::GetTrigger(Command::Key::LockOn) || Pad::GetTrigger(Command::GamePad::LockOn)) {
		result.push_back(pressLockOnCommand_);
	}
	// 回避コマンド
	if (Keyboard::GetTrigger(Command::Key::Evasion) || Pad::GetTrigger(Command::GamePad::Evasion)) {
		result.push_back(pressEvasionCommand_);
	}

	return result;
}

void InputHandler::AssignNormalAttackCommand() {
	ICommand* command = new NormalAttackCommand();
	this->pressNormalAttackCommand_ = command;
}

void InputHandler::AssignParryCommand() {
	ICommand* command = new ParryCommand();
	this->pressParryCommand_ = command;
}

void InputHandler::AssignLockOnCommand() {
	ICommand* command = new LockOnCommand();
	this->pressLockOnCommand_ = command;
}

void InputHandler::AssignEvasionCommand() {
	ICommand* command = new EvasionCommand();
	this->pressEvasionCommand_ = command;
}