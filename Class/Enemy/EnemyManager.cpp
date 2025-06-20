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
	isDefeatedAllEnemy_ = false;
	isStartWave_ = false;

	jsonSaiji_.Init("SaijiParameter.json");

	jsonSaiji_.BeginGroup("SaijiParameter")
		.AddValue<float>("EnemyDist", &enemyDist_)
		.AddValue<float>("EnemyDist", &attackEnemyDist_)
		.BeginGroup("Idle")
		.AddValue<float>("StandTime", &SaijiState::IdleParameter::standTime)
		.AddValue<float>("FollowingDist", &SaijiState::IdleParameter::followingDist)
		.EndGroup()
		.BeginGroup("Move")
		.AddValue<float>("AttackDist", &SaijiState::MoveParameter::attackDist)
		.AddValue<float>("RunTime", &SaijiState::MoveParameter::runTime)
		.AddValue<float>("DefaultSpeed", &SaijiState::MoveParameter::defaultSpeed)
		.EndGroup()
		.BeginGroup("Attack")
		.AddValue<float>("StartAcceptTime", &SaijiState::AttackParameter::startAcceptTime)
		.AddValue<float>("EndAcceptTime", &SaijiState::AttackParameter::endAcceptTime)
		.EndGroup()
		.BeginGroup("Spacing")
		.AddValue<float>("SpacingTime", &SaijiState::SpacingParameter::spacingTime)
		.AddValue<float>("SpacingDist", &SaijiState::SpacingParameter::spaceDist)
		.EndGroup()
		.BeginGroup("Following")
		.AddValue<float>("IdleDist", &SaijiState::FollowingParameter::idleDist)
		.EndGroup()
		.BeginGroup("HitReaction")
		.AddValue<float>("Decay", &SaijiState::HitReactionParameter::decay)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

	parameterEditor_.Initialize();

}

void EnemyManager::Finalize()
{
	//リストをクリア
	ClearList();
}

void EnemyManager::Update()
{

	spawnData_.Update();

	uint16_t closenessCounter = 0;

	CheckIsSpawn();

	//死亡した敵を削除
	enemies_.remove_if([](IEnemy* enemy) {

		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}

		return false;

		});

	//全ての敵の更新
	for (auto enemyA = enemies_.begin(); enemyA != enemies_.end(); enemyA++) {

		//プレイヤーとの押し出し処理
		{

			//二人の距離を測る
			Vector3 diff = (*enemyA)->GetPosition() - *player_->GetModelPos();
			//Y軸移動は考えない
			diff.y = 0.0f;
			//距離
			float dist = diff.Length();
			//押し出し判定距離
			float judgeDist = playerDist_;

			//距離が一定以上近い場合、押し出しベクトルを加算する
			if (dist < judgeDist && dist > 0.0001f) {
				(*enemyA)->AddRepulsiveForce(diff.Normalize() * ((judgeDist - dist) / judgeDist));
			}

		}

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
	case EnemyType::kSaiji:
		enemies_.push_back(new Saiji());
		break;
	case EnemyType::kOniHayha:
		enemies_.push_back(new OniHayha());
		break;
	case EnemyType::kBoss:
		enemies_.push_back(new Boss());
		break;
	default:
		break;
	}

	//初期化してリストに追加
	enemies_.back()->Initialize(player_, position, camera_, this);
	enemies_.back()->SetParameter(parameterEditor_.GetParameter(type));

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

	//スポーンポイント設定
	ImGui::DragFloat3("spawn point", &spawnPoint_.x, 0.1f);

	//ウェーブ開始ボタン
	if (ImGui::Button("Start Wave")) {
		StartWave();
	}

	//敵の数が最大数になるまで召喚可能
	if (enemies_.size() < kMaxEnemyCount_) {

		//ボス召喚
		if (ImGui::Button("Create Boss")) {
			CreateEnemy(spawnPoint_, EnemyType::kBoss);
		}

		//ザコ召喚
		if (ImGui::Button("Create Saiji")) {
			CreateEnemy(spawnPoint_, EnemyType::kSaiji);
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

			jsonSaiji_.DebugGUI();

			ImGui::EndTabItem();

		}

		//敵の共通パラメータを設定する
		if (ImGui::BeginTabItem("Default Parameter")) {

			parameterEditor_.DebugGUI();

			ImGui::EndTabItem();

		}

		//敵の共通ステート変数をいじる
		if (ImGui::BeginTabItem("SpawnData")) {

			spawnData_.DebugGUI();

			ImGui::Checkbox("ShowModel", &isShowSpawnDataModel_);

			ImGui::EndTabItem();

		}

		ImGui::EndTabBar();
	}

}

void EnemyManager::SortAscendingDistanceList()
{

	enemies_.sort([](IEnemy* enemyA, IEnemy* enemyB) {
		return enemyA->GetDistFromPlayer() < enemyB->GetDistFromPlayer();
		});

}

const EnemyAttackParameter& EnemyManager::GetEnemyAttackParameter(const std::string& name) const
{
	
	for (auto enemy = enemies_.begin(); enemy != enemies_.end(); enemy++) {
		//同一の名前があったらそれを返す
		if ((*enemy)->GetColliderName() == name) {
			return (*enemy)->GetAttackParameter();
		}

	}

	//存在しない場合はありえないのでassert
	assert(false);

	return enemies_.back()->GetAttackParameter();

}

void EnemyManager::SpawnFromWaveData(WaveData& waveData)
{

	for (EnemyData& enemyData : waveData.GetEnemyData()) {

		CreateEnemy(enemyData.position, enemyData.type);

	}

}

void EnemyManager::CheckIsSpawn()
{

	//ウェーブ開始状態で敵リストが空だったら次のウェーブに移行
	if (isStartWave_ and enemies_.empty()) {
		//ウェーブカウント増加
		spawnData_.AddWaveCount();

		//全ウェーブ終了していない場合
		if (not spawnData_.IsEndAllWave()) {
			//現在のウェーブの敵データからスポーンさせる
			SpawnFromWaveData(spawnData_.GetCurrentWaveData());
		}
		//全ウェーブ終了していたら
		else {
			EndGame();
		}

	}

}

void EnemyManager::StartWave()
{
	//敵リストクリア
	ClearList();
	//ウェーブカウントリセット
	spawnData_.ResetWaveCount();
	isShowSpawnDataModel_ = false;
	//スポーンデータのモデルを非表示
	spawnData_.SetIsShowModel(isShowSpawnDataModel_);
	//ウェーブ開始
	isStartWave_ = true;
	//現在のウェーブの敵データからスポーンさせる
	SpawnFromWaveData(spawnData_.GetCurrentWaveData());

}

bool EnemyManager::IsAnyAttack()
{

	//誰かが攻撃していたらtrueを返す
	for (auto enemy = enemies_.begin(); enemy != enemies_.end(); enemy++) {

		if ((*enemy)->GetIsAttack()) {
			return true;
		}

	}

	return false;
}

uint16_t EnemyManager::GetAttackPhaseCount()
{
	
	uint16_t count = 0;

	//攻撃態勢に入っている人数をカウント
	for (auto enemy = enemies_.begin(); enemy != enemies_.end(); enemy++) {

		if ((*enemy)->GetIsAttackPhase()) {
			count++;
		}

	}

	return count;

}

void EnemyManager::EndGame()
{
	//ウェーブ開始フラグを降ろす
	isStartWave_ = false;
	//全ての敵が倒されたフラグを立てる
	isDefeatedAllEnemy_ = true;

}

