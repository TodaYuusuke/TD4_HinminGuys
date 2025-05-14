#pragma once
#include "../ISystem.h"
#include "State/IMoveSystemState.h"

/// <summary>
/// 自機の移動機能をまとめたクラス
/// </summary>
class Move : public ISystem {
public:
	// コンストラクタ
	Move(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Move() override;

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
	/// 移動の状態を更新
	/// </summary>
	void MoveState();

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="pState">次の状態</param>
	void ChangeState(IMoveSystemState* pState);

	// 絶対値に変換
	LWP::Math::Vector3 Abs(LWP::Math::Vector3 value) {
		LWP::Math::Vector3 result{
			std::fabsf(value.x),
			std::fabsf(value.y),
			std::fabsf(value.z)
		};
		return result;
	}

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 移動対象のモデルのアドレスを取得
	/// </summary>
	/// <returns></returns>
	LWP::Resource::RigidModel GetModel() { return *model_; }

	/// <summary>
	/// 移動速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetMoveVel() { return moveVel_; }
	/// <summary>
	/// 向いている方向を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetMoveQuat() { return quat_; }
	/// <summary>
	/// 向いている方向を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetMoveRadian() { return radian_; }
	/// <summary>
	/// 移動状態を取得
	/// </summary>
	/// <returns></returns>
	IMoveSystemState* GetMoveState() { return state_; }
	/// <summary>
	/// 移動しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsMove() { return isMove_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 移動対象のモデルのアドレスを設定
	/// </summary>
	/// <param name="model">モデルのアドレス</param>
	void SetModel(LWP::Resource::RigidModel* model) { model_ = model; }

	/// <summary>
	/// 移動速度を設定
	/// </summary>
	/// <param name="moveVel">移動速度</param>
	void SetMoveVel(const LWP::Math::Vector3& moveVel) { moveVel_ = moveVel; }
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
	/// <summary>
	/// 移動速度の倍率を設定
	/// </summary>
	/// <param name="moveMultiply"></param>
	void SetMoveMultiply(const float& moveMultiply) { moveMultiply_ = moveMultiply; }
#pragma endregion

private:// jsonで保存する値
	// 歩き時の速度倍率
	float walkSpeedMultiply = 1.0f;
	// 走り時の速度倍率
	float dashSpeedMultiply = 1.0f;

private:// プライベートな変数
	// 移動対象のモデルのアドレス
	LWP::Resource::RigidModel* model_;

	IMoveSystemState* state_;

	// 移動速度
	LWP::Math::Vector3 moveVel_;

	// 向いている角度
	LWP::Math::Quaternion quat_ = { 0.0f,0.0f,0.0f,1.0f };
	LWP::Math::Vector3 radian_;

	// 移動時のイージング
	LWP::Math::Vector3 moveOffset_;

	// 移動速度の倍率
	float moveMultiply_;

	// 移動しているか
	bool isMove_;
};