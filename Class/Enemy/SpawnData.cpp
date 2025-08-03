#include "SpawnData.h"
#include <string>

SpawnData::SpawnData()
{
	currentWaveCount_ = startWaveCount_;
	waveData_.clear();
	Load();
}

SpawnData::~SpawnData()
{
	waveData_.clear();
}

void SpawnData::Update()
{

	waveData_.remove_if([](WaveData& waveData) {

		//削除フラグ立っていたら消す
		if (waveData.GetIsDelete()) {
			return true;
		}

		return false;

		});

	for (WaveData& waveData : waveData_) {

		waveData.Update();

	}

}

void SpawnData::DebugGUI()
{

	//セーブ
	if (ImGui::Button("Save")) {
		Save();
	}

	//ロード
	if (ImGui::Button("Load")) {
		Load();
	}

	ImGui::DragFloat3("SpawnPoint", &spawnPoint_.x, 0.1f);

	//ウェーブ追加
	if (ImGui::Button("Add Wave")) {
		WaveData data{};
		waveData_.push_back(data);
	}

	//全ウェーブデータのデバッグ表示
	for (int32_t i = startWaveCount_; WaveData& waveData : waveData_) {

		//各ウェーブを区別するため数字をふる
		std::string treeName = "Wave" + std::to_string(i);

		//ツリーウィンドウ表示
		if (ImGui::TreeNode(treeName.c_str())) {

			//追加ボタン
			if (ImGui::Button("Add Enemy")) {
				waveData.AddEnemyData(spawnPoint_);
			}
			//モデル表示フラグ
			ImGui::Checkbox("Show Model", &waveData.isActive);

			//ウェーブ削除用ツリー
			if (ImGui::TreeNode("Delete Wave")) {
				//ウェーブ削除
				if (ImGui::Button("Delete Wave")) {
					waveData.DeleteWave();
				}

				ImGui::TreePop();
			}

			//デバッグ表示
			waveData.DebugGUI();

			ImGui::TreePop();

		}

		i++;

	}

}

WaveData& SpawnData::GetCurrentWaveData()
{
	
	for (int32_t i = startWaveCount_; WaveData& waveData : waveData_) {

		//現在のウェーブカウントとループ数が一致していたらそのデータを返す
		if (i == currentWaveCount_) {
			return waveData;
		}

		i++;

	}

	//データが範囲外なのでassert出す
	assert(false);
	//仮の返り値
	return waveData_.back();

}

void SpawnData::SetIsShowModel(bool flag)
{

	for (WaveData& waveData : waveData_) {
		waveData.isActive = flag;
	}

}


void SpawnData::Save()
{

	json_.Init("EnemySpawn.json");

	//全ウェーブの保存
	for (int32_t i = startWaveCount_; WaveData& waveData : waveData_) {

		std::string num = std::to_string(i);
		num.insert(0, 4 - num.length(), '0');

		//ウェーブ+ウェーブ番号+ウェーブ内の敵数の順に文字列を作る
		std::string groupName = "Wave|" + num;

		json_.BeginGroup(groupName);

		waveData.AddValue(json_);

		json_.EndGroup();

		i++;

	}
	
	json_.Save();

	//メッセージ出す
	MessageBox(nullptr, L"セーブしました。", L"SpawnData - Save", 0);

}


void SpawnData::Load()
{

	waveData_.clear();

	// 初期化
	json_.Init("EnemySpawn.json");
	json_.CheckJsonFile();

	// グループ名の取得
	LWP::Utility::NestedList nameList = LWP::Utility::JsonIO::LoadGroupNames("EnemySpawn.json");

	// グループ名リストが空の場合早期リターン
	if (nameList.empty()) {
		return;
	}

	for (auto itr = nameList.begin(); itr != nameList.end(); ++itr) {
		// 名前を' | 'で分割する
		std::vector<std::string> splitName = LWP::Utility::Split(itr->name, '|');

		// 最初にWaveと書かれている場合
		if (splitName[0] == "Wave") {
			
			json_.BeginGroup(itr->name);

			WaveData waveData{};
			waveData_.push_back(waveData);
			waveData_.back().Load(json_, itr->list);

			json_.EndGroup();

			// 次のループへ
			continue;
		}

	}

	json_.Load();

	//ロードしてから実際のタイプをセット
	for (WaveData& waveData : waveData_) {
		waveData.SetType();
	}

	//現在のウェーブカウントリセット
	currentWaveCount_ = startWaveCount_;

}
