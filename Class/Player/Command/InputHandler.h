#pragma once
#include "../Adapter/Adapter.h"
#include "ICommand.h"
#include <map>
#include <set>

class InputHandler {
public:
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

private:
	// 通常攻撃コマンド
	ICommand* pressNormalAttackCommand_;
	// パリィコマンド
	ICommand* pressParryCommand_;
	// ロックオンコマンド
	ICommand* pressLockOnCommand_;
	// 回避コマンド
	ICommand* pressEvasionCommand_;

	// 実行用のコマンド
	std::vector<ICommand*> commands_;
};