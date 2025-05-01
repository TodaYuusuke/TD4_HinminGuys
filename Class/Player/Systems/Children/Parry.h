#pragma once
#include "../ISystem.h"
#include "../EventOrder.h"

/// <summary>
/// 自機のパリィ機能をまとめたクラス
/// </summary>
class Parry : public ISystem {
public:
	// コンストラクタ
	Parry(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Parry() override = default;

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

	/// <summary>
	/// ImGuiによるパラメータ表示
	/// </summary>
	void DebugGUI();

	/// <summary>
	/// パリィのコマンド
	/// </summary>
	void Command();

public:// Getter, Setter
#pragma region Getter

#pragma endregion

private:// 定数
	// パリィ発動までにかかる時間
	const float kSwingTime = 60.0f * 0.0f;
	// 通常パリィの猶予時間[frame * 秒]
	const float kGoodParryTime = 60.0f * 0.6f;
	// ジャストパリィの猶予時間[frame * 秒]
	const float kJustParryTime = 60.0f * 0.1f;
	// パリィの硬直[frame * 秒]
	const float kRecoveryTime = 60.0f * 0.0f;

private:
	// フレーム単位で発生するアクションイベントを管理するクラス
	EventOrder eventOrder_;

	// パリィ判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;
};