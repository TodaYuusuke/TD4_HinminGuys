#pragma once
#include "IFollowCameraState.h"

class Player;
/// <summary>
///	対象をロックオン中の状態
/// </summary>
class LockOnCamera : public IFollowCameraState {
public:
	// コンストラクタ
	LockOnCamera(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~LockOnCamera() override = default;

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
	LWP::Math::Vector3 lockOnOffset_;
	LWP::Math::Vector3 radian_;

	float rate_ = 0.05f;

	float t_;
};