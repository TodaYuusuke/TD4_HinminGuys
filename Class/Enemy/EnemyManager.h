#pragma once
#include "../Adapter/Adapter.h"
#include "IEnemy.h"
#include "Childs/Normal/Normal.h"
#include "Childs/Boss/Boss.h"
#include <list>
#include <memory>

class Player;

/// <summary>
/// 全ての敵を管理するクラス
/// </summary>
class EnemyManager
{
public:

	~EnemyManager();

	//初期化
	void Initialize();
	//終了処理
	void Finalize();
	//更新
	void Update();
	//プレイヤーをセット
	void SetPlayer(Player* player) { player_ = player; }
	//敵追加、配置
	void CreateEnemy(const Vector3& position, EnemyType type);
	//リストのクリア
	void ClearList();
	//デバッグウィンドウ
	void DebugGUI();
	//リスト取得
	std::list<IEnemy*>* GetEnemyListPtr() { return &enemies_; }
	//リストを距離順にソート(昇順。後半になるにつれて距離が長くなる)
	void SortAscendingDistanceList();

private:

	Player* player_;

	//全ての敵を管理するリスト
	std::list<IEnemy*> enemies_;

#pragma region デバッグ用

	//スポーン座標指定
	Vector3 spawnPoint_;

#pragma endregion

	//敵が互いに取る距離
	float enemyDist_ = 1.0f;

};

