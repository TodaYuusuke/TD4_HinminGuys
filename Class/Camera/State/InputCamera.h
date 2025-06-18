#pragma once
#include "IFollowCameraState.h"

class Player;
/// <summary>
/// 入力によってカメラを動かせる状態
/// </summary>
class InputCamera : public IFollowCameraState {
public:
	struct EasingData {
		LWP::Math::Vector3 start;
		LWP::Math::Vector3 end;
		float t;
		bool isActive;
	};

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
	/// <summary>
	/// カメラの角度を初期位置に戻す(入力がない場合)
	/// </summary>
	void ReturnRotate();

	/// <summary>
	/// 追従対象とカメラの距離をデフォルトに戻す処理をしない
	/// </summary>
	void FinishReturnTargetDistance();
	/// <summary>
	/// カメラの角度を初期位置に戻す開始
	/// </summary>
	void StartReturnRotate();

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

	// カメラ角度を戻す
	EasingData returnRot_;
};