#pragma once
#include "../Adapter/Adapter.h"
#include "IEnemy.h"
#include <list>

/// <summary>
/// 敵スポーン情報
/// </summary>
struct  EnemyData {
	LWP::Resource::RigidModel model; //表示用モデル
	std::string typeStr{}; //種類の名前タグ(保存用)
	LWP::Math::Vector3 position; //位置
	EnemyType type; //種類
	float delayTime; //出現遅延時間(秒)
	bool isDelete; //削除フラグ
};

/// <summary>
/// ウェーブ1つ分の情報
/// </summary>
class WaveData
{
public:
	WaveData();
	~WaveData();

	//更新
	void Update();

	//デバッグ用のGUI表示
	void DebugGUI();
	//ウェーブに敵追加
	void AddEnemyData(const LWP::Math::Vector3& position);
	//ウェーブを削除する
	void DeleteWave() { isDelete_ = true; }
	//削除フラグ取得
	bool GetIsDelete() const { return isDelete_; }
	//jsonの値追加
	void AddValue(LWP::Utility::JsonIO& json);
	//ウェーブ内の敵の数を取得
	int32_t GetEnemyCount() { return int32_t(enemyData_.size()); }
	//ロード
	void Load(LWP::Utility::JsonIO& json, LWP::Utility::NestedList& list);

private:

	//敵のスポーン地点
	std::list<EnemyData> enemyData_;
	//削除フラグ
	bool isDelete_ = false;
	//アクティブフラグ
	bool isActive_ = true;

};
