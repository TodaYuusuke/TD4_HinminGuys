#pragma once
#include "../Adapter/Adapter.h"
#include "WaveData.h"
#include <list>

/// <summary>
/// ゲーム全体の敵出現データ管理
/// </summary>
class SpawnData
{
public:
	SpawnData();
	~SpawnData();

	//更新
	void Update();

	//デバッグ用のGUI表示
	void DebugGUI();

private:

	//保存
	void Save();
	//読み込み
	void Load();

private:

	//敵出現リスト
	std::list<WaveData> waveData_;

	// jsonによるパラメータの保存、読み込み
	LWP::Utility::JsonIO json_;
	//敵追加座標
	LWP::Math::Vector3 spawnPoint_;
	//現在のウェーブ数
	int32_t currentWaveCount_;

};


