#include "EnemyManager.h"
#include "../Player/Player.h"
#include "States.h"

EnemyManager::~EnemyManager()
{
	Finalize();
}

void EnemyManager::Initialize()
{
	//リストをクリア
	ClearList();
	spawnPoint_ = { 0.0f,0.0f,0.0f };
	IEnemy::ResetAttack();
}

void EnemyManager::Finalize()
{
	//リストをクリア
	ClearList();
}

void EnemyManager::Update()
{

	uint16_t closenessCounter = 0;

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
			//押し出し判定距離
			float judgeDist = enemyDist_;

			//近い敵は押し出し判定を小さくする
			if (closenessCounter < IEnemy::GetMaxAttackCount()) {
				judgeDist = attackEnemyDist_;
			}

			//距離が一定以上近い場合、押し出しベクトルを加算する
			if (dist < judgeDist && dist > 0.0001f) {
				(*enemyA)->AddRepulsiveForce(diff.Normalize() * ((judgeDist - dist) / judgeDist));
			}

		}
		(*enemyA)->SetClosenessCount(closenessCounter);
		(*enemyA)->Update();

		closenessCounter++;

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
		enemies_.push_back(new Normal());
		break;
	case EnemyType::kBoss:
		enemies_.push_back(new Boss());
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

void EnemyManager::DebugGUI()
{

	ImGui::Begin("Enemy Manager");

	//スポーンポイント設定
	ImGui::DragFloat3("spawn point", &spawnPoint_.x, 0.1f);

	//敵の数が最大数になるまで召喚可能
	if (enemies_.size() < kMaxEnemyCount_) {

		//ボス召喚
		if (ImGui::Button("Create Boss")) {
			CreateEnemy(spawnPoint_, EnemyType::kBoss);
		}

		//ザコ召喚
		if (ImGui::Button("Create Normal")) {
			CreateEnemy(spawnPoint_, EnemyType::kNormal);
		}

	}
	else {

		//敵が上限である旨のテキストを出す
		ImGui::Text("Enemy Count Maximum");

	}

	//敵リセット
	if (ImGui::Button("Clear")) {
		ClearList();
	}

	//敵の総数
	ImGui::Text("enemy Count : %d", enemies_.size());

	//敵それぞれのデータやステートのパラメータ調整
	if (ImGui::BeginTabBar("Enemy Manager Item List", ImGuiTabBarFlags_None)) {

		//全ての敵のデバッグ
		if (ImGui::BeginTabItem("Enemies")) {

			//デバッグ
			for (auto enemy = enemies_.begin(); enemy != enemies_.end(); enemy++) {
				(*enemy)->DebugGUI();
			}

			ImGui::EndTabItem();

		}

		//敵の共通ステート変数をいじる
		if (ImGui::BeginTabItem("State Parameter")) {

			//マネージャーで取っている敵の距離調整
			ImGui::DragFloat("enemyDist", &enemyDist_, 0.1f);
			ImGui::DragFloat("attackEnemyDist", &attackEnemyDist_, 0.1f);

			//各ステートのパラメータ調整
			for (int32_t i = 0; i < int32_t(States::kMax); i++) {
				DebugState(States(i));
			}

			ImGui::EndTabItem();

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

void EnemyManager::DebugState(States states)
{

	switch (states)
	{
	case States::kNormalIdle:
		NormalIdle::DebugGUI();
		break;
	case States::kNormalMove:
		NormalMove::DebugGUI();
		break;
	case States::kNormalAttack:
		NormalAttack::DebugGUI();
		break;
	case States::kSpacing:
		Spacing::DebugGUI();
		break;
	case States::kFollowing:
		Following::DebugGUI();
		break;
	case States::kWaitingForAttack:
		WaitingForAttack::DebugGUI();
		break;
	default:
		break;
	}

}

