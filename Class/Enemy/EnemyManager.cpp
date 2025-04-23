#include "EnemyManager.h"
#include "../Player/Player.h"

EnemyManager::~EnemyManager()
{
	Finalize();
}

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
	for (auto enemyA = enemies_.begin(); enemyA != enemies_.end(); enemyA++) {
		//互いの情報を共有するためのループ
		for (auto enemyB = enemies_.begin(); enemyB != enemies_.end(); enemyB++) {

			//同一なら無視
			if (enemyA == enemyB) {
				continue;
			}

			//二人の距離を測る
			Vector3 diff = (*enemyA)->GetPosition() - (*enemyB)->GetPosition();
			//Y軸移動は考えない
			diff.y = 0.0f;
			//距離
			float dist = diff.Length();

			//距離が一定以上近い場合、押し出しベクトルを加算する
			if (dist < enemyDist_ && dist > 0.0001f) {
				(*enemyA)->AddRepulsiveForce(diff.Normalize() * ((enemyDist_ - dist) / enemyDist_));
			}

		}

		(*enemyA)->Update();
	}

	SortAscendingDistanceList();

}

void EnemyManager::CreateEnemy(const Vector3& position, EnemyType type)
{

	//プレイヤーが渡されていない場合、エラーメッセージを出す
	if (not player_) {
		MessageBox(nullptr, L"Player not set", L"EnemyManager - AddEnemy", 0);
		assert(false);
		return;
	}

	//タイプに応じて生成するものを変更
	switch (type)
	{
	case EnemyType::kNormal:
		enemies_.push_back(new NormalEnemy());
		break;
	case EnemyType::kBoss:
		enemies_.push_back(new BossEnemy());
		break;
	default:
		break;
	}

	//初期化してリストに追加
	enemies_.back()->Initialize(player_, position);
	
}



void EnemyManager::ClearList()
{
	
	//リストを空にする
	while (not enemies_.empty()) {

		//deleteしてからpop
		delete enemies_.back();
		enemies_.pop_back();

	}

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

	if (ImGui::BeginTabBar("Enemies", ImGuiTabBarFlags_None)) {

		//デバッグ
		for (auto enemy = enemies_.begin(); enemy != enemies_.end(); enemy++) {
			(*enemy)->Debug();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

}

void EnemyManager::SortAscendingDistanceList()
{

	enemies_.sort([](IEnemy* enemyA, IEnemy* enemyB) {
		return enemyA->GetDistFromPlayer() < enemyB->GetDistFromPlayer();
		});

}

