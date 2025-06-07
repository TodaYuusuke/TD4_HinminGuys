#pragma once
#include "../Adapter/Adapter.h"
#include "BanInput.h"

class Player;
class ICommand {
public:
	virtual ~ICommand() = default;
public:
	bool isActive_ = false;
};

/// <summary>
/// 移動コマンド
/// </summary>
class MoveCommand : public ICommand {
public:
	MoveCommand();
};
/// <summary>
/// 攻撃コマンド
/// </summary>
class NormalAttackCommand : public ICommand {
public:
	NormalAttackCommand();
};
/// <summary>
/// パリィコマンド
/// </summary>
class ParryCommand : public ICommand {
public:
	ParryCommand();
};
/// <summary>
/// ロックオンコマンド
/// </summary>
class LockOnCommand : public ICommand {
public:
	LockOnCommand();
};
/// <summary>
/// 回避コマンド
/// </summary>
class EvasionCommand : public ICommand {
public:
	EvasionCommand();
};
/// <summary>
/// 鞘コマンド
/// </summary>
class SheathCommand : public ICommand {
public:
	SheathCommand();
};