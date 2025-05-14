#pragma once
#include "../Adapter/Adapter.h"
#include "IEnemy.h"
#include <list>

/// <summary>
/// 敵スポーン情報
/// </summary>
struct EnemySpawnData {
	LWP::Math::Vector3 position; //位置
	EnemyType type; //種類
	float delayTime; //出現遅延時間(フレーム)
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

private:

	//敵のスポーン地点
	std::list<EnemySpawnData> spawnData_;


};
