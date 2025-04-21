#pragma once
#include "../ISystem.h"

/// <summary>
/// 自機の移動機能をまとめたクラス
/// </summary>
class Move : public ISystem {
public:
	// コンストラクタ
	Move(LWP::Object::Camera* camera);
	// デストラクタ
	~Move() override = default;

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

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="up"></param>
	/// <returns></returns>
	float GetAngle(const LWP::Math::Vector3& a, const LWP::Math::Vector3& b, const LWP::Math::Vector3& up);

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
	/// 向いている方向を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetRotate() { return rotate_; }
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
	/// <param name="rotate">向かせる方向</param>
	void SetRotate(const LWP::Math::Quaternion& rotate) { rotate_ = rotate; }
#pragma endregion

private:// プライベートな変数
	// 移動対象のモデルのアドレス
	LWP::Resource::RigidModel* model_;

	// 移動速度
	LWP::Math::Vector3 moveVel_;

	// 向いている角度
	LWP::Math::Quaternion rotate_ = {0.0f,0.0f,0.0f,1.0f};

	// 移動時のイージング
	LWP::Math::Vector3 moveOffset_;
};