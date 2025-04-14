#pragma once
#include "ICharacter.h"
#include "Systems/MoveSystem.h"
#include "Systems/ParrySystem.h"
#include <memory>

class Player : public ICharacter {
public:
	// コンストラクタ
	Player() = default;
	// デストラクタ
	~Player() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 全ての数値をリセットする
	/// </summary>
	void Reset();

private:
	/// <summary>
	/// 自機機能を全て生成
	/// </summary>
	void CreateSystems();

private:
	// 移動機能
	std::unique_ptr<MoveSystem> moveSystem_;
	// パリィ機能
	std::unique_ptr<ParrySystem> parrySystem_;
};