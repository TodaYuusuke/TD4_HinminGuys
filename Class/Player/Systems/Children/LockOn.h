#pragma once
#include "../ISystem.h"
#include "../../../Enemy/IEnemy.h"
#include <vector>

class FollowCamera;
class LockOn : public ISystem {
public:// 構造体
	// ロックオン時に必要な情報
	struct LockOnData {
		IEnemy* enemyData;	// 敵の情報
		bool isActive;		// 一度ロックオンしたか
	};

public:
	// コンストラクタ
	LockOn(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~LockOn() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 全ての数値をリセット
	/// </summary>
	void Reset() override;

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
	/// ロックオン対象を変える
	/// </summary>
	void ChangeLockOnTarget();

	/// <summary>
	/// ロックオン可能範囲の敵を索敵
	/// </summary>
	void SearchLockOnEnemy();

	/// <summary>
	/// ロックオン情報を初期化する
	/// </summary>
	void ClearLockOn();

	/// <summary>
	/// 最も近い敵を索敵
	/// </summary>
	void SearchNearEnemy();

	/// <summary>
	/// ロックオン開始時に必要な情報
	/// </summary>
	void StartLockOn(IEnemy* enemy);

	// カメラの正面方向を算出
	LWP::Math::Vector2 ConvertWorld2Screen(LWP::Math::Vector3 worldPos);

	bool IsObjectInScreen(LWP::Math::Vector3 worldPos);

	bool IsObjectInOppositeDirection(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraDirection);

	LWP::Math::Vector3 Transforms(const LWP::Math::Vector3& vector, const LWP::Math::Matrix4x4& matrix);

public:// Getter, Setter
#pragma region Getter

#pragma endregion

#pragma region Setter
	/// <summary>
	/// 敵のリストを設定
	/// </summary>
	/// <param name="enemyList">敵のリスト</param>
	void SetEnemyList(std::list<IEnemy*>* enemyList) { enemies_ = enemyList; }

	/// <summary>
	/// 追従カメラのポインタを設定
	/// </summary>
	/// <param name="followCamera">追従カメラのポインタ</param>
	void SetFollowCamera(FollowCamera* followCamera) { followCamera_ = followCamera; }
#pragma endregion

private:// 定数
	// ロックオンの上限数
	inline constexpr static int32_t kMaxLockOnNum = 10;

	// ロックオンできる範囲
	constexpr static float kMaxRange = 50.0f;

private:// 外部からポインタをもらう変数
	// 敵のリスト
	const std::list<IEnemy*>* enemies_;
	// 追従カメラ
	FollowCamera* followCamera_;

private:
	// ロックオンされたことのある敵のID
	std::vector<int32_t> lockedEnemyIDs_;

	// 現在ロックオンされている敵の情報
	IEnemy* lockOnEnemy_;

	// ロックオン可能数
	int lockOnNum_;

	// カメラ回転、左右どちらに入力があったかを受け取る
	float inputCameraRotateY_;

	// ロックオン対象を変えるか
	bool isChangeLockOn_;
	bool isChangeLocked_;
};