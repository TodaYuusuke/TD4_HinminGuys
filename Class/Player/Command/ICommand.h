#pragma once
#include "../Adapter/Adapter.h"
#include "BanInput.h"

class Player;
class ICommand {
public:
	virtual ~ICommand() = default;
	virtual void Exec(Player& player, int& banInput) = 0;
	virtual void Reset(Player& player, int& banInput) = 0;
	inline void SetBanInput(const int& banInput) { banInput_ = banInput; }
	inline int GetBanInput() { return banInput_; }
public:
	// 入力不可状態
	int banInput_;
	int currentInput_;

	bool isActive_;
};

/// <summary>
/// 移動コマンド
/// </summary>
class MoveCommand : public ICommand {
public:
	MoveCommand();
	void Exec(Player& player, int& banInput)override;
	void Reset(Player& player, int& banInput) override;
};
/// <summary>
/// 攻撃コマンド
/// </summary>
class NormalAttackCommand : public ICommand {
public:
	NormalAttackCommand();
	void Exec(Player& player, int& banInput)override;
	void Reset(Player& player, int& banInput) override;
};
/// <summary>
/// パリィコマンド
/// </summary>
class ParryCommand : public ICommand {
public:
	ParryCommand();
	void Exec(Player& player, int& banInput)override;
	void Reset(Player& player, int& banInput) override;
};
/// <summary>
/// ロックオンコマンド
/// </summary>
class LockOnCommand : public ICommand {
public:
	void Exec(Player& player, int& banInput)override;
	void Reset(Player& player, int& banInput) override;
};
/// <summary>
/// 回避コマンド
/// </summary>
class EvasionCommand : public ICommand {
public:
	EvasionCommand();
	void Exec(Player& player, int& banInput)override;
	void Reset(Player& player, int& banInput) override;
};
/// <summary>
/// 鞘コマンド
/// </summary>
class SheathCommand : public ICommand {
public:
	SheathCommand();
	void Exec(Player& player, int& banInput)override;
	void Reset(Player& player, int& banInput) override;
};