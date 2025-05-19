#pragma once
#include "../ISystem.h"
#include "State/ISheathSystemState.h"

class Sheath : public ISystem {
public:
	// コンストラクタ
	Sheath(LWP::Object::Camera * camera, Player * player);
	// デストラクタ
	~Sheath() override = default;

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
	/// 鞘を投げるor鞘に向かってダッシュコマンド
	/// </summary>
	void Command();

	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="pState">次の状態</param>
	void ChangeState(ISheathSystemState* pState);

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 鞘のWorldTFを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat GetSheathWorldTF() { return sheathModel_.worldTF; }
	/// <summary>
	/// 移動速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 向いている方向を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetQuat() { return quat_; }
	/// <summary>
	/// 向いている方向を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetRadian() { return radian_; }
	/// <summary>
	/// 鞘の状態を取得
	/// </summary>
	/// <returns></returns>
	ISheathSystemState* GetSheathState() { return state_; }
#pragma endregion

#pragma region Setter
	void SetSheathPos(const LWP::Math::Vector3& pos) { sheathModel_.worldTF.translation = pos; }
	/// <summary>
	/// 移動速度を設定
	/// </summary>
	/// <param name="velocity">移動速度</param>
	void SetVelocity(const LWP::Math::Vector3& velocity) { velocity_ = velocity; }
	/// <summary>
	/// 向いている方向を設定
	/// </summary>
	/// <param name="radian">向かせる方向(ラジアン)</param>
	void SetRotate(const LWP::Math::Vector3& radian) { radian_ = radian; }
	/// <summary>
	/// 向いている方向を設定
	/// </summary>
	/// <param name="quat">向かせる方向(クォータニオン)</param>
	void SetRotate(const LWP::Math::Quaternion& quat) { quat_ = quat; }
#pragma endregion

private:// プライベートな変数
	// 状態遷移
	ISheathSystemState* state_;

	// 鞘のモデル
	LWP::Resource::RigidModel sheathModel_;

	// 移動速度
	LWP::Math::Vector3 velocity_;
	// 向いている角度
	LWP::Math::Quaternion quat_ = { 0.0f,0.0f,0.0f,1.0f };
	LWP::Math::Vector3 radian_;
};