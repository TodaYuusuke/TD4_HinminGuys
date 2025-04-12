#pragma once
#include "../Adapter/Adapter.h"
#include "IEnemy.h"
#include <list>
#include <memory>

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
	//敵追加
	void AddEnemy(std::unique_ptr<IEnemy> enemy);
	//リストのクリア
	void ClearList();

private:
	//全ての敵を管理するリスト
	std::list<std::unique_ptr<IEnemy>> enemies_;

};

