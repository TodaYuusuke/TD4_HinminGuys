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
	int size = (int)commands_.size();
	ImGui::Begin("InputHandler");
	ImGui::Text("InputNum : %d", size);
	ImGui::End();
}

void InputHandler::CreateCommand() {
	AssignNormalAttackCommand();
	AssignParryCommand();
	AssignLockOnCommand();
}

std::vector<ICommand*> InputHandler::HandleInput() {
	
	std::vector<ICommand*> result;

	// 通常攻撃コマンド
	if (Keyboard::GetTrigger(Key::NormalAttack) || Pad::GetPress(GamePad::NormalAttack)) {
		result.push_back(pressNormalAttackCommand_);
	}
	// パリィコマンド
	if (Keyboard::GetTrigger(Key::Parry) || Pad::GetPress(GamePad::Parry)) {
		result.push_back(pressParryCommand_);
	}
	// ロックオンコマンド
	if (Keyboard::GetTrigger(Key::LockOn) || Pad::GetPress(GamePad::LockOn)) {
		result.push_back(pressLockOnCommand_);
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