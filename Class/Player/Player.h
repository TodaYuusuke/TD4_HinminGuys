#pragma once
#include "ICharacter.h"
#include "Systems/Children/Move.h"
#include "Systems/Children/Parry.h"
#include "Systems/Children/Attack.h"
#include <memory>

class Player : public ICharacter {
public:
	// コンストラクタ
	Player(LWP::Object::Camera* camera);
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
	/// <summary>
	/// シーンで使用しているカメラのポインタを設定
	/// </summary>
	/// <param name="camera">カメラのアドレス</param>
	void SetCamera(LWP::Object::Camera* camera) { pCamera_ = camera; }
#pragma endregion

private:
	// 移動機能
	std::unique_ptr<Move> moveSystem_;
	// パリィ機能
	std::unique_ptr<Parry> parrySystem_;
	// 攻撃機能
	std::unique_ptr<Attack> attackSystem_;
};