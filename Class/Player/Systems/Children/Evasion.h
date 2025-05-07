#pragma once
#include "../ISystem.h"

class Evasion : public ISystem {
public:
	// コンストラクタ
	Evasion(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Evasion() override = default;

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
	/// 回避のコマンド
	/// </summary>
	void Command();

private:
	/// <summary>
	/// アクションイベントの生成
	/// </summary>
	void CreateEventOrder();

	/// <summary>
	/// 回避の状態を確認
	/// </summary>
	void CheckEvasionState();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 回避速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 向いている方向を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetRadian() { return radian_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:// jsonで保存する値
	// 回避発動までにかかる時間[秒]
	float kEvasionSwingTime = 0.0f;
	// 回避の無敵時間[秒]
	float kInvinsibleTime = 0.6f;
	// 回避の硬直[秒]
	float kEvasionRecoveryTime = 0.0f;
	// 回避速度の係数
	float moveMultiply = 1.0f;

private:// プライベートな変数
	// 回避時の速度
	LWP::Math::Vector3 velocity_;
	// 回避時の角度(ラジアン)
	LWP::Math::Vector3 radian_;
};