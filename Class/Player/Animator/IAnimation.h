#pragma once

class Player;
class IAnimation {
public:
	virtual ~IAnimation() = default;
	virtual void Exec(Player& player) = 0;
};

/// <summary>
/// 移動コマンド
/// </summary>
class MoveAnim : public IAnimation {
public:
	void Exec(Player& player)override;
};
/// <summary>
/// 攻撃コマンド
/// </summary>
class AttackAnim : public IAnimation {
public:
	void Exec(Player& player)override;
};
/// <summary>
/// パリィコマンド
/// </summary>
class ParryAnim : public IAnimation {
public:
	void Exec(Player& player)override;
};
/// <summary>
/// 回避コマンド
/// </summary>
class EvasionAnim : public IAnimation {
public:
	void Exec(Player& player)override;
};
/// <summary>
/// 鞘コマンド
/// </summary>
class SheathAnim : public IAnimation {
public:
	void Exec(Player& player)override;
};