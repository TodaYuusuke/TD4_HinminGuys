#pragma once
#include "../ISystem.h"
#include "../../../Enemy/IEnemy.h"
#include <vector>

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

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// ロックオン可能範囲の敵を索敵
	/// </summary>
	void SearchLockOnEnemy();
	/// <summary>
	/// 最も近い敵を索敵
	/// </summary>
	void SearchNearEnemy();

	/// <summary>
	/// ロックオンリストを初期化する
	/// </summary>
	void ClearLockOnList();

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
	void SetEnemyList(std::list<std::unique_ptr<IEnemy>>* enemyList) { enemies_ = enemyList; }
#pragma endregion

private:// 定数
	// ロックオンの上限数
	inline constexpr static int32_t kMaxLockOnNum = 10;

	// ロックオンできる範囲
	constexpr static float kMaxRange = 50.0f;

private:
	// ロックオン対象の敵
	std::vector<LockOnData> lockOnDatas_;
	// ロックオン対象のID
	std::vector<int32_t> lockedEnemyIDs_;

	// 敵のリスト
	const std::list<std::unique_ptr<IEnemy>>* enemies_;

	LWP::Math::Vector3 lockedEnemyPos_;

	// 最も近くにいる敵の距離
	LWP::Math::Vector3 minDistance_;
};