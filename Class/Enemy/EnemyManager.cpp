#include "EnemyManager.h"

void EnemyManager::Initialize()
{
	//リストをクリア
	ClearList();
}

void EnemyManager::Finalize()
{
	//リストをクリア
	ClearList();
}

void EnemyManager::Update()
{

	//全ての敵の更新
	for (const std::unique_ptr<IEnemy>& enemy : enemies_) {
		enemy->Update();
	}

}

void EnemyManager::AddEnemy(std::unique_ptr<IEnemy> enemy)
{
	enemies_.push_back(std::move(enemy));
}

void EnemyManager::ClearList()
{
	//リストを空にする
	enemies_.clear();
}
