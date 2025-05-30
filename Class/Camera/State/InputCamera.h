#pragma once
#include "IFollowCameraState.h"

/// <summary>
/// 鞘を投擲可能状態
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

private:
	// さや投げの移動量
	LWP::Math::Vector3 throwMovement_ = { 0,0,10 };

	LWP::Math::Vector3 velocity_;
	LWP::Math::Vector3 start_;
	LWP::Math::Vector3 end_;
	float t_;
};