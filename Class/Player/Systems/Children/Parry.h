#pragma once
#include "../ISystem.h"

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
	void DebugGUI() override;

	/// <summary>
	/// jsonファイルの作成
	/// </summary>
	void CreateJsonFIle() override;

	/// <summary>
	/// パリィのコマンド
	/// </summary>
	void Command();

	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand();

private:
	/// <summary>
	/// 当たり判定を作成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// アクションイベントを生成
	/// </summary>
	void CreateEventOrder();

	/// <summary>
	/// パリィの状態を確認
	/// </summary>
	void CheckParryState();

public:// Getter, Setter
#pragma region Getter

#pragma endregion

private:// jsonで保存する値
	// パリィ発動までにかかる時間[秒]
	static float kSwingTime;
	// 通常パリィの猶予時間[秒]
	static float kGoodParryTime;
	// ジャストパリィの猶予時間[秒]
	static float kJustParryTime;
	// パリィの硬直[秒]
	static float kRecoveryTime;

private:
	// パリィ判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;

	bool isJustParry_;
	bool isGoodParry_;
};