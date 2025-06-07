#pragma once
#include "../Adapter/Adapter.h"
#include "ICommand.h"

class Player;
class InputHandler {
public:
	// シングルトン
	static InputHandler* GetInstance();

	InputHandler() = default;
	~InputHandler() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ImGuiによるパラメータを表示
	/// </summary>
	void DebugGUI();

private:
	/// <summary>
	/// 全てのコマンドを作成
	/// </summary>
	void CreateCommand();

	/// <summary>
	/// コマンドの入力状況の確認
	/// </summary>
	/// <returns></returns>
	std::vector<ICommand*> HandleInput();

	/// <summary>
	/// 移動のコマンドを登録
	/// </summary>
	void AssignMoveCommand();
	/// <summary>
	/// 通常攻撃のコマンドを登録
	/// </summary>
	void AssignNormalAttackCommand();
	/// <summary>
	/// パリィコマンドの登録
	/// </summary>
	void AssignParryCommand();
	/// <summary>
	/// ロックオンコマンドの登録
	/// </summary>
	void AssignLockOnCommand();
	/// <summary>
	/// 回避コマンドの登録
	/// </summary>
	void AssignEvasionCommand();
	/// <summary>
	/// 鞘コマンドの登録
	/// </summary>
	void AssignSheathCommand();

public:
	ICommand* GetMoveCommand() { return pressMoveCommand_; }
	ICommand* GetAttackCommand() { return pressAttackCommand_; }
	ICommand* GetParryCommand() { return pressParryCommand_; }
	ICommand* GetLockOnCommand() { return pressLockOnCommand_; }
	ICommand* GetEvasionCommand() { return pressEvasionCommand_; }
	ICommand* GetSheathCommand() { return pressSheathCommand_; }

	bool GetPressMove() {
		for (ICommand* cmd : commands_) {
			if (cmd == pressMoveCommand_) { return true; }
		}
		return false;
	}

	bool GetPressAttack() {
		for (ICommand* cmd : commands_) {
			if (cmd == pressAttackCommand_) { return true; }
		}
		return false;
	}

	bool GetPressParry() {
		for (ICommand* cmd : commands_) {
			if (cmd == pressParryCommand_) { return true; }
		}
		return false;
	}

	bool GetPressLockOn() {
		for (ICommand* cmd : commands_) {
			if (cmd == pressLockOnCommand_) { return true; }
		}
		return false;
	}

	bool GetPressEvasion() {
		for (ICommand* cmd : commands_) {
			if (cmd == pressEvasionCommand_) { return true; }
		}
		return false;
	}

	bool GetPressSheath() {
		for (ICommand* cmd : commands_) {
			if (cmd == pressSheathCommand_) { return true; }
		}
		return false;
	}

private:
	// 実行用のコマンド
	std::vector<ICommand*> commands_;

	// 移動コマンド
	ICommand* pressMoveCommand_;
	// 攻撃コマンド
	ICommand* pressAttackCommand_;
	// パリィコマンド
	ICommand* pressParryCommand_;
	// ロックオンコマンド
	ICommand* pressLockOnCommand_;
	// 回避コマンド
	ICommand* pressEvasionCommand_;
	// 鞘コマンド
	ICommand* pressSheathCommand_;
};