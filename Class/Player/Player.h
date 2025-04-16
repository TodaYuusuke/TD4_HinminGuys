#pragma once
#include "ICharacter.h"
#include "Systems/MoveSystem.h"
#include "Systems/ParrySystem.h"
#include "Systems/AttackSystem.h"
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

	/// <summary>
	/// デバッグ用のタブを表示(Debug時のみ)
	/// </summary>
	void DebugWindow();

public:// Getter,Setter
#pragma region Getter
	LWP::Math::Vector3* GetModelPos() { return &model_.worldTF.translation; }
	LWP::Object::TransformQuat* GetWorldTF() { return &model_.worldTF; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// 移動機能
	std::unique_ptr<MoveSystem> moveSystem_;
	// パリィ機能
	std::unique_ptr<ParrySystem> parrySystem_;
	// 攻撃機能
	std::unique_ptr<AttackSystem> attackSystem_;
};