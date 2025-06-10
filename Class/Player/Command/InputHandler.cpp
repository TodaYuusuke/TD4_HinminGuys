#include "InputHandler.h"
#include "InputConfig.h"
#include "../Player.h"

using namespace LWP::Input;
using namespace InputConfig;

InputHandler* InputHandler::GetInstance() {
	static InputHandler instance;
	return &instance;
}

void InputHandler::Initialize() {
	// コマンド作成
	CreateCommand();
}

void InputHandler::Update(Player& player) {
	// 入力されたコマンドを確認
	commands_ = HandleInput(player);

	for (ICommand* command : commands_) {
		command->Exec(player);
	}
}

void InputHandler::DebugGUI() {
	if (ImGui::TreeNode("Command")) {
		// 登録されているコマンド
		if (ImGui::TreeNode("AllView")) {

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
	AssignSheathCommand();
	AssignMoveCommand();
}

std::vector<ICommand*> InputHandler::HandleInput(Player& player) {
	std::vector<ICommand*> result;

	// ロックオンコマンド
	if (Keyboard::GetTrigger(Command::Key::LockOn) || Pad::GetTrigger(Command::GamePad::LockOn)) {
		result.push_back(pressLockOnCommand_);
	}

	// 鞘機能稼働中で抜刀後状態以外なら何もしない
	if (player.GetSystemManager()->GetSheathSystem()->GetIsActive()) {
		if (player.GetSystemManager()->GetSheathSystem()->GetSheathState()->GetStateName() != "SwordDrawn") {
			return result;
		}
	}

	// 通常攻撃コマンド
	if ((Keyboard::GetTrigger(Command::Key::NormalAttack) || Pad::GetTrigger(Command::GamePad::NormalAttack))) {
		result.push_back(pressAttackCommand_);
	}
	// パリィコマンド(鞘を回収できる状態の時はパリィできない)
	if ((Keyboard::GetTrigger(Command::Key::Parry) || Pad::GetTrigger(Command::GamePad::Parry)) && !player.GetSystemManager()->GetSheathSystem()->GetIsNone()) {
		result.push_back(pressParryCommand_);
	}
	// 回避コマンド
	if (Keyboard::GetTrigger(Command::Key::Evasion) || Pad::GetTrigger(Command::GamePad::Evasion)) {
		result.push_back(pressEvasionCommand_);
	}
	// 鞘コマンド
	if (Keyboard::GetTrigger(Command::Key::Sheath) || Pad::GetTrigger(Command::GamePad::Sheath)) {
		result.push_back(pressSheathCommand_);
	}
	// 移動入力
	LWP::Math::Vector2 dir = Pad::GetLStick();
	if (Keyboard::GetPress(Command::Key::Move::Up) || Keyboard::GetPress(Command::Key::Move::Down) || Keyboard::GetPress(Command::Key::Move::Left) || Keyboard::GetPress(Command::Key::Move::Right)
		|| dir.Length() != 0.0f) {
		result.push_back(pressMoveCommand_);
	}

	return result;
}

void InputHandler::AssignMoveCommand() {
	ICommand* command = new MoveCommand();
	this->pressMoveCommand_ = command;
}

void InputHandler::AssignNormalAttackCommand() {
	ICommand* command = new NormalAttackCommand();
	this->pressAttackCommand_ = command;
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

void InputHandler::AssignSheathCommand() {
	ICommand* command = new SheathCommand();
	this->pressSheathCommand_ = command;
}