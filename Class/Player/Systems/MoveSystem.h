#pragma once
#include "ISystem.h"

/// <summary>
/// 自機の移動機能をまとめたクラス
/// </summary>
class MoveSystem : public ISystem {
public:
	// コンストラクタ
	MoveSystem() = default;
	// デストラクタ
	~MoveSystem() override = default;

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

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 移動速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetMoveVel() { return moveVel_; }
	/// <summary>
	/// 向いている方向を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetRotate() { return rotate_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 移動速度を設定
	/// </summary>
	/// <param name="moveVel">移動速度</param>
	void SetMoveVel(const LWP::Math::Vector3& moveVel) { moveVel_ = moveVel; }
	/// <summary>
	/// 向いている方向を設定
	/// </summary>
	/// <param name="rotate">向かせる方向</param>
	void SetRotate(const LWP::Math::Quaternion& rotate) { rotate_ = rotate; }
#pragma endregion

private:// プライベートな変数
	// 移動速度
	LWP::Math::Vector3 moveVel_;
	// 向いている角度
	LWP::Math::Quaternion rotate_;
};