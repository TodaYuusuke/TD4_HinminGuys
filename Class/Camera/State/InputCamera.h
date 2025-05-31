#pragma once
#include "IFollowCameraState.h"

/// <summary>
/// 入力によってカメラを動かせる状態
/// </summary>
class InputCamera : public IFollowCameraState {
public:
	// コンストラクタ
	InputCamera(FollowCamera* followCamera);
	// デストラクタ
	~InputCamera() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 角度の更新
	/// </summary>
	void RotateUpdate() override;

private:
	// スティックの入力を受け取る
	LWP::Math::Vector3 stickDir_;
};