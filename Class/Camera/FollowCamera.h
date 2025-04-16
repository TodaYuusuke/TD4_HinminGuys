#pragma once
#include "../Adapter/Adapter.h"

/// <summary>
/// 自機に追従するカメラ
/// </summary>
class FollowCamera {
public:
	// コンストラクタ
	FollowCamera() = default;
	// デストラクタ
	~FollowCamera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private:
	void InputUpdate();

	float LerpShortAngle(float a, float b, float t);

	LWP::Math::Vector3 CalcOffset() const;

	LWP::Math::Quaternion QuaternionLookRotation(LWP::Math::Vector3 from, LWP::Math::Vector3 to);

	LWP::Math::Vector3 Rotate(LWP::Math::Quaternion q,const LWP::Math::Vector3& v) const;

public:// Getter,Setter
#pragma region Getter
	/// <summary>
	/// カメラのアドレスを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::Camera* GetCamera() { return camera_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 追従対象の座標を設定
	/// </summary>
	/// <param name="targetPos">追従対象の座標のアドレス</param>
	void SetTargetPos(LWP::Math::Vector3* targetPos) { targetPos_ = targetPos; }

	/// <summary>
	/// Sceneで使用されているmainCameraのアドレスを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(LWP::Object::Camera* camera) { camera_ = camera; }

	//void SetTarget(LWP::Object::TransformEuler* target) { target_ = target; }
#pragma endregion

private:// 定数
	///
	/// 視点操作の設定
	/// 
	
	// x軸の最低値
	const float kMinAxisX = 0.2f;
	// x軸の最大値
	const float kMaxAxisX = 0.4f;
	// 入力感度
	const LWP::Math::Vector2 kSensitivity = { 0.05f, 0.02f };
	// 視点移動の滑らかさ(0~1の間で設定)
	const float kRotationSmoothness = 0.2f;
	// カメラの後追い速度(0~1の間で設定)
	float kFollowRate = 1.0f;

	// 追従対象との距離
	LWP::Math::Vector3 kTargetDist = { 0.0f,0.0f,-50.0f };

	// 初期角度 
	LWP::Math::Vector3 kStartAngle = { 0.3f, 0.0f, 0.0f };

private:
	// カメラ
	LWP::Object::Camera* camera_;

	LWP::Object::TransformEuler target_;

	// 追従対象の残像座標
	LWP::Math::Vector3 interTarget_;
	// 目標角度
	LWP::Math::Vector2 destinationAngle_;

	// 追従対象の座標
	const LWP::Math::Vector3* targetPos_;
};

