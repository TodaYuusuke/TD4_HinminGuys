#pragma once
#include "../ISystem.h"

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
	void DegubGui();

private:
	// 攻撃判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;
};