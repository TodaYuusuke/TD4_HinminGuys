#pragma once
#include "../Adapter/Adapter.h"
#include "IEnemy.h"
#include "Childs/Normal/NormalEnemy.h"
#include "Childs/Boss/BossEnemy.h"
#include <list>
#include <memory>

class Player;

/// <summary>
/// 全ての敵を管理するクラス
/// </summary>
class EnemyManager
{
public:

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
	void Debug();

private:

	Player* player_;

	//全ての敵を管理するリスト
	std::list<std::unique_ptr<IEnemy>> enemies_;

#pragma region デバッグ用

	//スポーン座標指定
	Vector3 spawnPoint_;

#pragma endregion

	//敵が互いに取る距離
	float enemyDist_ = 1.0f;

};

