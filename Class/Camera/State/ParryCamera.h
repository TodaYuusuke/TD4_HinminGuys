#pragma once
#include "IFollowCameraState.h"
#include "../../Components/Shake.h"
#include "../../Components/EventOrder.h"

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

private:// 数学関数
	/// <summary>
	/// 回転角を方向ベクトルに変換
	/// </summary>
	/// <param name="rotate">オイラー角</param>
	/// <returns></returns>
	LWP::Math::Vector3 RotateToDir(const LWP::Math::Vector3& rotate);

	/// <summary>
	/// 右左を判定する関数
	/// </summary>
	/// <param name="forward"></param>
	/// <param name="targetDir"></param>
	/// <param name="up"></param>
	/// <returns></returns>
	int DetermineSide3D(const LWP::Math::Vector3& forward, const LWP::Math::Vector3& targetDir, const LWP::Math::Vector3& up);

private:// 外部から受け取る変数
	Player* player_;

private:
	// 揺れ
	Shake shake_;
	// カメラの揺れ
	LWP::Math::Vector3 shakeOffset_;
	LWP::Math::Vector3 shakeRange_;

	// 追従対象との距離をイージングで調整
	EventOrder targetDistOrder_;
	// ロックオン対象とカメラの距離
	LWP::Math::Vector3 lockOnOffset_;

	// 角度
	LWP::Math::Vector3 radian_;
	LWP::Math::Quaternion quat_;
	LWP::Math::Vector3 start_;
	LWP::Math::Vector3 end_;
	LWP::Math::Vector3 targetPos_;
	LWP::Math::Vector3 startTargetPos_;
	float t_;

	float timeScale_;
	float rate_;
	float currentFrame_;
};