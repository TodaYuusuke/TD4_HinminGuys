#pragma once
#include "IFollowCameraState.h"
#include "../../Shake/Shake.h"
#include "../../Player/Systems/EventOrder.h"

class Player;
/// <summary>
///	対象をロックオン中の状態
/// </summary>
class ParryCamera : public IFollowCameraState {
public:
	// コンストラクタ
	ParryCamera(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~ParryCamera() override;

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

	/// <summary>
	/// ロックオン対象とカメラの距離の更新
	/// </summary>
	void TargetDistUpdate();

	/// <summary>
	/// 追従対象との距離をイージングするためのイベント生成
	/// </summary>
	void CreateEventOrder();

private:
	float zoomFinishTime = 22.0f;
	float zoomHoldFinishTime = 0.0f;
	float returnFinishTime = 39.0f;

private:// 外部から受け取る変数
	Player* player_;

private:
	// 追従対象との距離をイージングで調整
	EventOrder targetDistOrder_;
	// 揺れ
	Shake shake_;

	// カメラの揺れ
	LWP::Math::Vector3 shakeOffset_;
	LWP::Math::Vector3 shakeRange_;

	// ロックオン対象とカメラの距離
	LWP::Math::Vector3 lockOnOffset_;

	// 
	LWP::Math::Vector3 radian_;
	LWP::Math::Quaternion pitch_; // X: 上下
	LWP::Math::Quaternion yaw_; // Y: 左右
	LWP::Math::Vector3 damping_;

	float rate_;
	float currentFrame_;
};