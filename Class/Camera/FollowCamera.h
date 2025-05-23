#pragma once
#include "../Adapter/Adapter.h"
#include <numbers>

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
	FollowCamera(LWP::Object::Camera* camera, LWP::Math::Vector3* targetPos);
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

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// ロックオンの更新処理
	/// </summary>
	void LockOnUpdate();

	/// <summary>
	/// 角度制限の処理
	/// </summary>
	/// <param name="target">制限対象</param>
	/// <param name="distance">二点間の距離</param>
	/// <param name="minLimitAngle">角度の下限値(単位:ラジアン)</param>
	/// <param name="maxLimitAngle">角度の上限値(単位:ラジアン)</param>
	void ClampAngle(float& target, LWP::Math::Vector3 distance, float minLimitAngle, float maxLimitAngle);

public:// Getter,Setter
#pragma region Getter
	/// <summary>
	/// カメラのアドレスを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::Camera* GetCamera() { return camera_; }
	/// <summary>
	/// 線形補間をしていない純粋なカメラ座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetDefaultPos() { return defaultPos_; }
	/// <summary>
	/// ロックオン中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsLockOn() { return lockOnData_.isLocked; }
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

#pragma endregion

private:// jsonで保存する値
	// 追従対象との距離
	LWP::Math::Vector3 kTargetDist = { 0.0f,0.0f,-20.0f };
	// 初期角度 
	LWP::Math::Vector3 kStartAngle = { 0.3f, 0.0f, 0.0f };
	// x軸の下限値
	float kMinRotateX = (float)std::numbers::pi / 2.0f - 0.1f;
	// x軸の上限値
	float kMaxRotateX = (float)std::numbers::pi / 2.0f + (float)std::numbers::pi / 4.0f;
	// カメラの感度
	float sensitivity = 1.0f;
	// カメラの補間レート
	float interTargetRate = 0.5f;

	float maxLength = 50.0f;

	// 回転角の始点
	const float kOriginRotateX = 90.0f;

private:// 外部からポインタをもらう変数
	// カメラ
	LWP::Object::Camera* camera_;

private:
	// 追従対象の座標
	const LWP::Math::Vector3* targetPos_;

	//　ロックオン時のカメラの位置調整
	LWP::Math::Vector3 lockOnOffset_;
	LWP::Math::Vector3 interTarget_;
	LWP::Math::Vector3 defaultPos_;

	// 追従対象との初期距離
	LWP::Math::Vector3 defaultTargetDist_;

	// ロックオン時に使う情報
	LockOnData lockOnData_;

	// パラメーターの保存
	LWP::Utility::JsonIO json_;

	LWP::Math::Vector3 radian_;
	LWP::Math::Quaternion quat_;
};

