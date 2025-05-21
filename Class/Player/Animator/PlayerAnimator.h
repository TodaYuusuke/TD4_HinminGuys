#pragma once
#include "../Adapter/Adapter.h"
#include "IAnimation.h"

class PlayerAnimator {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LWP::Resource::Animation* animation);

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
	std::vector<IAnimation*> HandleAnimation(Player& player);

	/// <summary>
	/// 移動のコマンドを登録
	/// </summary>
	void AssignMoveCommand();
	/// <summary>
	/// 攻撃のコマンドを登録
	/// </summary>
	void AssignAttackCommand();
	/// <summary>
	/// パリィコマンドの登録
	/// </summary>
	void AssignParryCommand();
	/// <summary>
	/// 回避コマンドの登録
	/// </summary>
	void AssignEvasionCommand();
	/// <summary>
	/// 鞘コマンドの登録
	/// </summary>
	void AssignSheathCommand();

private:
	// 移動コマンド
	IAnimation* moveAnim_;
	// 通常攻撃コマンド
	IAnimation* attackAnim_;
	// パリィコマンド
	IAnimation* parryAnim_;
	// 回避コマンド
	IAnimation* evasionAnim_;
	// 鞘コマンド
	IAnimation* sheathAnim_;

	// 実行用のコマンド
	std::vector<IAnimation*> animations_;

	// 実行中のアニメーション
	LWP::Resource::Animation* animation_;
};