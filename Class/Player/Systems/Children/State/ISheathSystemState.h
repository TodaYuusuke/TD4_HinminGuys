#pragma once
#include "../Adapter/Adapter.h"

class Player;
class Sheath;
class ISheathSystemState {
public:
	virtual ~ISheathSystemState() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// コマンド
	/// </summary>
	virtual void Command() = 0;

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 攻撃時の移動ベクトルを取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 攻撃時の体の角度を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetRadian() { return radian_; }
	/// <summary>
	/// 攻撃時の体の角度を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetQuat() { return quat_; }
	/// <summary>
	/// 状態の名前を取得
	/// </summary>
	/// <returns></returns>
	std::string GetStateName() { return stateName_; }
	/// <summary>
	/// 機能が使える状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActive_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 状態の名前を設定
	/// </summary>
	/// <param name="stateName"></param>
	void SetStateName(const std::string& stateName) { stateName_ = stateName; }
	/// <summary>
	/// 機能が使える状態かを設定
	/// </summary>
	void SetIsActive(bool isActive) { isActive_ = isActive; }
#pragma endregion

protected:
	Player* player_;
	Sheath* sheathSystem_;

	// 状態の名前
	std::string stateName_;

	LWP::Math::Vector3 velocity_;
	LWP::Math::Vector3 radian_;
	LWP::Math::Quaternion quat_;

	bool isActive_;
};