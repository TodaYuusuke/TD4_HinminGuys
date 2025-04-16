#pragma once
#include "ISystem.h"

/// <summary>
/// 自機の攻撃機能をまとめたクラス
/// </summary>
class AttackSystem : public ISystem{
public:
	// コンストラクタ
	AttackSystem() = default;
	// デストラクタ
	~AttackSystem() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 全ての数値をリセット
	/// </summary>
	void Reset() override;

private:

};