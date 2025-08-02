#pragma once
#include "../Adapter/Adapter.h"
#include "State/IFollowCameraState.h"
#include "FovSystem.h"
#include <numbers>

class Player;
/// <summary>
/// 自機に追従するカメラ
/// </summary>
class FollowCamera {
public:
	struct LockOnData {
		// ロックオン時のTransform
		LWP::Object::TransformQuat lockOnTransform;
		// ロックオン対象のTransformのポインタ
		LWP::Object::TransformQuat* targetTransform;
		// ロックオン時のカメラの位置調整
		LWP::Math::Vector3 rotateOffset;
		// ロックオン対象とカメラとの方向ベクトル
		LWP::Math::Vector3 t2cDir;
		// ロックオンしているか
		bool isLocked;
	};

public:
	// コンストラクタ
	FollowCamera(Player* player, LWP::Object::Camera* camera, LWP::Math::Vector3* targetPos);
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

	/// <summary>
	/// デバッグ用のタブを表示(Debug時のみ)
	/// </summary>
	void DebugGUI();

	/// <summary>
	/// カメラの状態を確認
	/// </summary>
	void CheckState();

	/// <summary>
	/// 視野角の更新処理
	/// </summary>
	void FovUpdate();

public:
	/// <summary>
	/// 角度制限の処理
	/// </summary>
	/// <param name="target">制限対象</param>
	/// <param name="distance">二点間の距離</param>
	/// <param name="minLimitAngle">角度の下限値(単位:ラジアン)</param>
	/// <param name="maxLimitAngle">角度の上限値(単位:ラジアン)</param>
	void ClampAngle(float& target, LWP::Math::Vector3 distance, float minLimitAngle, float maxLimitAngle);

	/// <summary>
	/// 状態の変更
	/// </summary>
	/// <param name="pState"></param>
	void ChangeState(IFollowCameraState* pState);

public:// アクセサ
	/// <summary>
	/// Fovイージング開始
	/// </summary>
	/// <param name="currentFov">現在のFov</param>
	/// <param name="goalFov">目標のFov</param>
	/// <param name="endFrame">終了時間</param>
	void StartFovEasing(const float& currentFov, const float& goalFov, const float& endFrame, const float& returnStayFrame);

#pragma region Getter
	/// <summary>
	/// カメラのアドレスを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::Camera* GetCamera() { return camera_; }
	/// <summary>
	/// ロックオン中の情報を取得
	/// </summary>
	/// <returns></returns>
	LockOnData GetLockOnData() { return lockOnData_; }
	/// <summary>
	/// Fovをイージングする機能を取得
	/// </summary>
	/// <returns></returns>
	FovSystem* GetFovSystem() { return fovSystem_.get(); }
	/// <summary>
	/// 線形補間をしていない純粋なカメラ座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetDefaultPos() { return defaultPos_; }
	/// <summary>
	/// 追従対象の座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetTargetPos() { return *targetPos_; }
	LWP::Math::Vector3 GetTargetPosition() { return targetPosition_; }
	LWP::Math::Vector3 GetRadian() { return radian_; }
	/// <summary>
	/// ロックオン中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsLockOn() { return lockOnData_.isLocked; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// Sceneで使用されているmainCameraのアドレスを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(LWP::Object::Camera* camera) { camera_ = camera; }
	/// <summary>
	/// 追従対象の座標を設定
	/// </summary>
	/// <param name="targetPos">追従対象の座標のアドレス</param>
	void SetTargetPos(LWP::Math::Vector3* targetPos) { targetPos_ = targetPos; }
	void SetTargetPosition(LWP::Math::Vector3 targetPos) { targetPosition_ = targetPos; }
	/// <summary>
	/// カメラの揺れを設定
	/// </summary>
	/// <param name="shakeOffset"></param>
	void SetShakeOffset(LWP::Math::Vector3 shakeOffset) { shakeOffset_ = shakeOffset; }
	/// <summary>
	/// カメラの角度を設定
	/// </summary>
	/// <returns></returns>
	void SetCameraRotate(LWP::Math::Quaternion quat) { camera_->worldTF.rotation = quat; }
	/// <summary>
	/// ラジアンを設定
	/// </summary>
	/// <param name="radian"></param>
	void SetRadian(LWP::Math::Vector3 radian) { radian_ = radian; }
#pragma endregion

	/// <summary>
	/// ロックオン開始
	/// </summary>
	/// <param name="lockOnTarget">ロックオン対象のアドレス</param>
	void StartLockOn(LWP::Object::TransformQuat* lockOnTarget) {
		lockOnData_.targetTransform = lockOnTarget;
		lockOnData_.isLocked = true;
	}
	/// <summary>
	/// ロックオン終了
	/// </summary>
	void FinishLockOn() {
		lockOnData_.targetTransform = nullptr;
		lockOnData_.isLocked = false;
	}

public:// jsonで保存する値
	// 追従対象との距離
	LWP::Math::Vector3 kTargetDist = { 0.0f,0.0f,-20.0f };
	// 初期角度 
	LWP::Math::Vector3 kStartAngle = { 0.3f, 0.0f, 0.0f };
	// パリィ後に操作がなかったときに戻す角度
	LWP::Math::Vector3 returnAngle = { 0.3f, 0.0f, 0.0f };

	// x軸の下限値
	float kMinRotateX = (float)std::numbers::pi / 2.0f - 0.1f;
	// x軸の上限値
	float kMaxRotateX = (float)std::numbers::pi / 2.0f + (float)std::numbers::pi / 4.0f;
	// カメラの感度
	float sensitivity = 1.0f;
	// カメラの補間レート
	float interTargetRate = 0.5f;

	float maxLength = 50.0f;

	// 追従対象との距離を補間するレート
	float targetDistRate = 0.1f;
	// カメラの回転補間レート
	float rotateRate = 0.1f;

	// 回転角の始点
	const float kOriginRotateX = 90.0f;

	// 追従対象との初期距離
	LWP::Math::Vector3 defaultTargetDist_;

	// 追従対象とカメラの距離をもとに戻すまでの時間
	float returnTargetDistTime = 20.0f;

	float returnRotateTime = 240.0f;
	// 注視点を変えたときにカメラを向けるまでの時間
	float switchTargetPosTime = 15.0f;

	// パリィ時のカメラのズーム時間
	float zoomFinishTime = 55.0f;
	float zoomHoldFinishTime = 33.0f;
	// パリィ時のカメラの揺れる時間
	float parryShakeTime = 22.0f;
	// ラジアルブラーの強度
	float parryBlurWidth = 0.003f;
	// パリィ時のヒットストップのする時間
	float parryHitStopTime = 0.25f;
	// パリィ時の角度
	LWP::Math::Vector3 parryAngle = { 0.0f,0.523f,0.06f };
	// パリィ時のカメラ距離
	LWP::Math::Vector3 parryDist = { 0.0f,0.0f,2.5f };
	// カメラの揺れる範囲
	LWP::Math::Vector3 parryShakeRange = { 0.01f,0.01f,0.0f };

public:// 外部からポインタをもらう変数
	// カメラ
	LWP::Object::Camera* camera_;

private:
	Player* player_;

private:
	std::unique_ptr<FovSystem> fovSystem_;

	// カメラの状態
	IFollowCameraState* state_;
	std::string preStateName_;

	// カメラの揺れ
	LWP::Math::Vector3 shakeOffset_;

	// 追従対象の座標
	const LWP::Math::Vector3* targetPos_;
	LWP::Math::Vector3 targetPosition_;

	//　ロックオン時のカメラの位置調整
	LWP::Math::Vector3 lockOnOffset_;
	LWP::Math::Vector3 interTarget_;
	// カメラの補間なし時の座標(自機の移動処理に使うために作成)
	LWP::Math::Vector3 defaultPos_;

	LWP::Math::Vector3 radian_;

	// ロックオン時に使う情報
	LockOnData lockOnData_;

	// 視野角の目標値
	float goalFov_;

	// パラメーターの保存
	LWP::Utility::JsonIO json_;
};

