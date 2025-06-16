#pragma once
#include "../Adapter/Adapter.h"
#include "BanInput.h"

class Player;
class ICommand {
public:
	// デストラクタ
	virtual ~ICommand() = default;

	/// <summary>
	/// コマンド実行
	/// </summary>
	virtual void Exec(Player& player) = 0;

public:

};

/// <summary>
/// 移動コマンド
/// </summary>
class MoveCommand : public ICommand {
public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	void Exec(Player& player) override;
};
/// <summary>
/// 攻撃コマンド
/// </summary>
class NormalAttackCommand : public ICommand {
public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	void Exec(Player& player) override;
};
/// <summary>
/// パリィコマンド
/// </summary>
class ParryCommand : public ICommand {
public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	void Exec(Player& player) override;
};
/// <summary>
/// ロックオンコマンド
/// </summary>
class LockOnCommand : public ICommand {
public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	void Exec(Player& player) override;
};
/// <summary>
/// 回避コマンド
/// </summary>
class EvasionCommand : public ICommand {
public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	void Exec(Player& player) override;
};
/// <summary>
/// 鞘コマンド
/// </summary>
class SheathCommand : public ICommand {
public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	void Exec(Player& player) override;
};