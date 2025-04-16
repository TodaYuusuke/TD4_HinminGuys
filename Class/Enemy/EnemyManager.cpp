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

void EnemyManager::CreateEnemy(const Vector3& position, EnemyType type)
{

	//プレイヤーが渡されていない場合、エラーメッセージを出す
	if (not player_) {
		MessageBox(nullptr, L"Player not set", L"EnemyManager - AddEnemy", 0);
		assert(false);
		return;
	}

	std::unique_ptr<IEnemy> enemy;

	//タイプに応じて生成するものを変更
	switch (type)
	{
	case EnemyType::kNormal:
		enemy = std::make_unique<NormalEnemy>();
		break;
	case EnemyType::kBoss:
		enemy = std::make_unique<BossEnemy>();
		break;
	default:
		break;
	}

	//初期化してリストに追加
	enemy->Initialize(player_, position);
	enemies_.push_back(std::move(enemy));

}



void EnemyManager::ClearList()
{
	//リストを空にする
	enemies_.clear();
}
