#pragma once
#include "IFollowCameraState.h"

class Player;
/// <summary>
/// 入力によってカメラを動かせる状態
/// </summary>
class InputCamera : public IFollowCameraState {
public:
	// コンストラクタ
	InputCamera(Player* player, FollowCamera* followCamera);
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
	Player* player_;

private:
	// スティックの入力を受け取る
	LWP::Math::Vector3 stickDir_;
	// 角度
	LWP::Math::Vector3 radian_;
	// 開始時のカメラと追従対象との距離
	LWP::Math::Vector3 startDist_;
	LWP::Math::Vector3 startTargetPos_;

	float t_;
};