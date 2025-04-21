#include "EnemyManager.h"

void EnemyManager::Initialize()
{
	//リストをクリア
	ClearList();
	spawnPoint_ = { 0.0f,0.0f,0.0f };
}

void EnemyManager::Finalize()
{
	//リストをクリア
	ClearList();
}

void EnemyManager::Update()
{

	//全ての敵の更新
	for (const std::unique_ptr<IEnemy>& enemyA : enemies_) {
		//互いの情報を共有するためのループ
		for (const std::unique_ptr<IEnemy>& enemyB : enemies_) {

			//同一なら無視
			if (enemyA == enemyB) {
				continue;
			}

			//二人の距離を測る
			Vector3 diff = enemyA->GetPosition() - enemyB->GetPosition();
			//Y軸移動は考えない
			diff.y = 0.0f;
			//距離
			float dist = diff.Length();

			//距離が一定以上近い場合、押し出しベクトルを加算する
			if (dist < enemyDist_ && dist > 0.0001f) {
				enemyA->AddRepulsiveForce(diff.Normalize() * ((enemyDist_ - dist) / enemyDist_));
			}

		}

		enemyA->Update();
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

void EnemyManager::Debug()
{

	ImGui::Begin("Enemy Manager");

	//スポーンポイント設定
	ImGui::DragFloat3("spawn point", &spawnPoint_.x, 0.1f);

	//ボス召喚
	if (ImGui::Button("Create Boss")) {
		CreateEnemy(spawnPoint_, EnemyType::kBoss);
	}

	//ザコ召喚
	if (ImGui::Button("Create Normal")) {
		CreateEnemy(spawnPoint_, EnemyType::kNormal);
	}

	ImGui::End();

}
