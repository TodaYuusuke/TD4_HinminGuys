#pragma once
#include "../ISystem.h"
#include "../EventOrder.h"

/// <summary>
/// 自機の攻撃機能をまとめたクラス
/// </summary>
class Attack : public ISystem{
public:
	// コンストラクタ
	Attack(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Attack() override = default;

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
	/// ImGuiによるパラメータを表示
	/// </summary>
	void DebugGUI();

	/// <summary>
	/// 通常攻撃のコマンド
	/// </summary>
	void NormalCommand();

private:// 定数
	// 通常攻撃発動までにかかる時間
	const float kNormalSwingTime = 60.0f * 0.0f;
	// 通常攻撃の猶予時間[frame * 秒]
	const float kNormalAttackTime = 60.0f * 0.6f;
	// 通常攻撃の硬直[frame * 秒]
	const float kNormalRecoveryTime = 60.0f * 0.2f;

private:
	// フレーム単位で発生するアクションイベントを管理するクラス
	EventOrder eventOrder_;

	// 攻撃判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;
};