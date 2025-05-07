#pragma once
#include "../Adapter/Adapter.h"

class Player;
class Attack;
class IAttackSystemState {
public:
	virtual ~IAttackSystemState() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 攻撃時の移動ベクトルを取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetAssistVelocity() { return assistVel_; }
	/// <summary>
	/// 攻撃時の体の角度を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetAssistRadian() { return assistRadian_; }
	/// <summary>
	/// 攻撃時の体の角度を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetAssistQuat() { return assistQuat_; }
#pragma endregion

#pragma region Setter

#pragma endregion

protected:
	Player* player_;
	Attack* attackSystem_;

	LWP::Math::Vector3 assistVel_;
	LWP::Math::Vector3 assistRadian_;
	LWP::Math::Quaternion assistQuat_;
};