#pragma once
#include "IFollowCameraState.h"
#include "../../Shake/Shake.h"

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

private:// 外部から受け取る変数
	Player* player_;

private:
	Shake shake_;

	// カメラの揺れ
	LWP::Math::Vector3 shakeOffset_;
	LWP::Math::Vector3 shakeRange_;

	// ロックオン対象とカメラの距離
	LWP::Math::Vector3 lockOnOffset_;
	// 
	LWP::Math::Vector3 radian_;

	float rate_;
	float currentFrame_;
};