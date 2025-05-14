#pragma once
#include "../Adapter/Adapter.h"

class Player;
class ICommand {
public:
	virtual ~ICommand() = default;
	virtual void Exec(Player& player) = 0;
};

/// <summary>
/// 攻撃コマンド
/// </summary>
class NormalAttackCommand : public ICommand {
public:
	void Exec(Player& player)override;
};
/// <summary>
/// パリィコマンド
/// </summary>
class ParryCommand : public ICommand {
public:
	void Exec(Player& player)override;
};
/// <summary>
/// ロックオンコマンド
/// </summary>
class LockOnCommand : public ICommand {
public:
	void Exec(Player& player)override;
};
/// <summary>
/// 回避コマンド
/// </summary>
class EvasionCommand : public ICommand {
public:
	void Exec(Player& player)override;
};