#include "InputHandler.h"
#include "InputConfig.h"

using namespace LWP::Input;
using namespace InputConfig;

InputHandler* InputHandler::GetInstance() {
	static InputHandler instance;
	return &instance;
}

void InputHandler::Initialize() {
	// コマンド作成
	CreateCommand();

	banInput_ = BanNone;
}

void InputHandler::Update(Player& player) {
	// 入力されたコマンドを確認
	//commands_ = HandleInput();

	// 削除
	//if (!currentCommand_.empty()) {
	//	currentCommand_[0]->Reset(player, banInput_);
	//	if (!currentCommand_[0]->isActive_) {
	//		currentCommand_.erase(currentCommand_.begin());
	//	}
	//}

	// set Command
	//for (ICommand* cmd : commands_) {
	//	cmd->Exec(player, banInput_);
	//}

	// 入力不可を更新
	//if (!currentCommand_.empty()) {
	//	banInput_ = currentCommand_[0]->banInput_;
	//}
	//else {
	//	banInput_ = BanNone;
	//}

	// 入力されたコマンドを確認
	commands_ = HandleInput();

	// set Command
	for (ICommand* cmd : commands_) {
		cmd->Exec(player, banInput_);

		if (cmd->isActive_) {
			currentCommand_ = cmd;
		}
	}

	if (currentCommand_) {
		currentCommand_->Reset(player, banInput_);
	}
}

void InputHandler::DebugGUI() {
	if (ImGui::TreeNode("Command")) {
		// 登録されているコマンド
		if (ImGui::TreeNode("AllView")) {

			ImGui::DragInt("BanBinary", &banInput_);
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

void InputHandler::AssignSheathCommand() {
	ICommand* command = new SheathCommand();
	this->pressSheathCommand_ = command;
}