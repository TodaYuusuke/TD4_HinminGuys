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
	/// <param name="player">自機のアドレス</param>
	void Update(Player& player);

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
	/// <summary>
	/// 入力可能なコマンドかを取得
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	bool CheckEnableCommand(const int& value) {
		for (ICommand* cmd : commands_) {
			if (IsBitSame(value, cmd->currentInput_, GetSetBitPosition(cmd->currentInput_))) {
				return true;
			}
		}
		return false;
	}

	int GetBanInput() { return banInput_; }

	/// <summary>
	/// 入力のあったコマンドを取得
	/// </summary>
	/// <returns></returns>
	std::vector<ICommand*> InputCommands() { return commands_; }

	ICommand* GetMoveCommand() { return pressMoveCommand_; }
	ICommand* GetAttackCommand() { return pressNormalAttackCommand_; }
	ICommand* GetParryCommand() { return pressParryCommand_; }
	ICommand* GetLockOnCommand() { return pressLockOnCommand_; }
	ICommand* GetEvasionCommand() { return pressEvasionCommand_; }
	ICommand* GetSheathCommand() { return pressSheathCommand_; }

private:
	// 実行用のコマンド
	std::vector<ICommand*> commands_;
	ICommand* currentCommand_;
	//std::vector<ICommand*> currentCommand_;

	// 移動コマンド
	ICommand* pressMoveCommand_;
	// 通常攻撃コマンド
	ICommand* pressNormalAttackCommand_;
	// パリィコマンド
	ICommand* pressParryCommand_;
	// ロックオンコマンド
	ICommand* pressLockOnCommand_;
	// 回避コマンド
	ICommand* pressEvasionCommand_;
	// 鞘コマンド
	ICommand* pressSheathCommand_;

	// 入力禁止状態
	int banInput_;
};