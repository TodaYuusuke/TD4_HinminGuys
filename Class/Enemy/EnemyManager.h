#pragma once
#include "../Adapter/Adapter.h"
#include "IEnemy.h"
#include "Normal/NormalEnemy.h"
#include "Boss/BossEnemy.h"
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

private:

	Player* player_;

	//全ての敵を管理するリスト
	std::list<std::unique_ptr<IEnemy>> enemies_;

};

