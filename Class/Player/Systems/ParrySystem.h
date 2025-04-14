#pragma once
#include "ISystem.h"

/// <summary>
/// 自機のパリィ機能をまとめたクラス
/// </summary>
class ParrySystem : public ISystem {
public:
	// コンストラクタ
	ParrySystem() = default;
	// デストラクタ
	~ParrySystem() override = default;

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