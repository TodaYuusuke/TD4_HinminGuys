#include "WaveData.h"

WaveData::WaveData()
{
	enemyData_.clear();
}

WaveData::~WaveData()
{
	enemyData_.clear();
}

void WaveData::Update()
{

	enemyData_.remove_if([](EnemyData& enemyData) {

		//削除フラグ立っていたら消す
		if (enemyData.isDelete) {
			return true;
		}

		return false;

		});

	for (EnemyData& enemyData : enemyData_) {
		enemyData.model.worldTF.translation = enemyData.position;
	}

}

void WaveData::DebugGUI()
{

	//敵全部
	for (int32_t i = 1; EnemyData& enemyData : enemyData_) {

		//各敵を区別するため数字をふる
		std::string treeName = "Enemy" + std::to_string(i);

		//敵ごとの変数調整
		if (ImGui::TreeNode(treeName.c_str())) {

			//Comboに利用するため、一時的にintに変換
			int currentType = static_cast<int>(enemyData.type);

			ImGui::DragFloat3("position", &enemyData.position.x, 0.1f);
			ImGui::DragFloat("delay Time (sec)", &enemyData.delayTime, 0.1f);
			if (ImGui::Combo("type", &currentType, IEnemy::enemyTypeName.data(), int(IEnemy::enemyTypeName.size()))) {
				//変更があった場合、切り替える
				enemyData.type = static_cast<EnemyType>(currentType);
				enemyData.typeStr = IEnemy::enemyTypeName[currentType];
			}

			//敵削除用ツリー
			if (ImGui::TreeNode("Delete Enemy")) {
				//ウェーブ削除
				if (ImGui::Button("Delete Enemy")) {
					enemyData.isDelete = true;
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		i++;

	}

}

void WaveData::AddEnemyData(const LWP::Math::Vector3& position)
{

	//新しいデータを作成し、初期化する
	EnemyData enemyData{};
	enemyData.typeStr = IEnemy::enemyTypeName[int(EnemyType::kNormal)];
	enemyData.type = EnemyType::kNormal;
	enemyData.position = position;
	enemyData.delayTime = 0;
	enemyData.isDelete = false;
	enemyData.model.LoadShortPath("player/Player_Simple.gltf");
	enemyData.model.worldTF.translation = position;
	enemyData.model.worldTF.scale = { 0.5f,0.5f,0.5f };

	enemyData_.push_back(enemyData);

}

void WaveData::AddValue(LWP::Utility::JsonIO& json)
{

	for (int32_t i = 1; EnemyData& enemyData : enemyData_) {

		//敵+敵番号+タイプ名の順に文字列作成
		std::string groupName = "Enemy|" + std::to_string(i);

		//jsonに追加
		json.BeginGroup(groupName)
			.AddValue("type", &enemyData.typeStr)
			.AddValue("position", &enemyData.position)
			.AddValue("delayTime", &enemyData.delayTime)
			.EndGroup();

		i++;

	}

}

void WaveData::Load(LWP::Utility::JsonIO& json, LWP::Utility::NestedList& nestedList)
{

	for (auto itr = nestedList.begin(); itr != nestedList.end(); ++itr) {
		// 名前を' | 'で分割する
		std::vector<std::string> splitName = LWP::Utility::Split(itr->name, '|');

		// 最初にWaveと書かれている場合
		if (splitName[0] == "Enemy") {

			EnemyData enemyData;
			enemyData.model.LoadShortPath("player/Player_Simple.gltf");

			enemyData_.push_back(enemyData);

			//jsonに追加
			json.BeginGroup(itr->name)
				.AddValue("type", &enemyData_.back().typeStr)
				.AddValue("position", &enemyData_.back().position)
				.AddValue("delayTime", &enemyData_.back().delayTime)
				.EndGroup();

			enemyData_.back().model.worldTF.translation = enemyData_.back().position;
			enemyData_.back().model.worldTF.scale = { 0.5f,0.5f,0.5f };
			enemyData_.back().isDelete = false;

			//タイプに応じて変更
			if (enemyData_.back().typeStr == IEnemy::enemyTypeName[int(EnemyType::kNormal)]) {
				enemyData_.back().type = EnemyType::kNormal;
			}
			else if (enemyData_.back().typeStr == IEnemy::enemyTypeName[int(EnemyType::kBoss)]) {
				enemyData_.back().type = EnemyType::kBoss;
			}

			// 次のループへ
			continue;
		}

	}

}

