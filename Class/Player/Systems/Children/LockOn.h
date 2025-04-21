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
	LockOn(LWP::Object::Camera* camera);
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

private:// 定数
	// ロックオンの上限数
	const int kMaxLockOnNum = 10;

	// ロックオンできる範囲
	const float kMaxRange = 50.0f;

private:
	// ロックオン対象の敵
	std::vector<LockOnData> lockOnDatas_;

	// 最も近くにいる敵の距離
	LWP::Math::Vector3 minDistance_;
};