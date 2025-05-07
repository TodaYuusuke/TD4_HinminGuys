#include "GameScene.h"
#include "Title.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

GameScene::GameScene() 
	: player_(&mainCamera, &enemyManager_, &followCamera_),
	  followCamera_(&mainCamera, player_.GetModelPos())
{
	enemyManager_.Initialize();
}

GameScene::~GameScene() {
	enemyManager_.Finalize();
}

// 初期化
void GameScene::Initialize() {
	// コマンドの登録
	inputHandler_.Initialize();

	// 敵管理クラス
	enemyManager_.Initialize();
	enemyManager_.SetPlayer(&player_);

	// 追従カメラの動作確認のため生成
	followCamera_.Initialize();

	// 自機の動作確認のため生成
	player_.Initialize();

#pragma region フィールドを一時的に生成
	// 一時的に平面を生成
	plane.LoadShortPath("field/ground/SimpleStage.gltf");
	plane.worldTF.scale = { 1000.0f,1000.0f ,1000.0f };
	plane.worldTF.translation = { 0,-1,0 };
	plane.materials["material"].uvTransform.scale = { 1000.0f,1000.0f ,0.0f };
	// 一時的に天球を生成
	skydome.LoadShortPath("field/skydome/SkyDome.gltf");
	skydome.worldTF.scale = { 1000.0f,1000.0f ,1000.0f };
	skydome.SetAllMaterialLighting(false);
#pragma endregion

	// 平行光源を配置(これも一時的に配置)
	light_.worldTF.translation = { 0,10,0 };
}

// 更新
void GameScene::Update() {
	// シーン遷移
	if (Keyboard::GetTrigger(DIK_P)) {
		nextSceneFunction = []() { return new Title(); };
	}

	// 入力されたコマンドを確認
	inputHandler_.Update(player_);

	//敵全て
	enemyManager_.Update();

	// 追従カメラ
	followCamera_.Update();

	// 自機
	player_.Update();

	// デバッグ用のウィンドウ
	DebugGUI();
}

void GameScene::DebugGUI() {
#ifdef _DEBUG
	ImGui::Begin("GameDebugWindow");

	// FPSカウンターの表示
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	// 自機
	if (ImGui::TreeNode("Player")) {
		player_.DebugGUI();
		ImGui::TreePop();
	}
	// キーコンフィグ
	if (ImGui::TreeNode("InputHandler")) {
		inputHandler_.DebugGUI();
		ImGui::TreePop();
	}
	// 追従カメラ
	if (ImGui::TreeNode("FollowCamera")) {
		followCamera_.DebugGUI();
		ImGui::TreePop();
	}
	// 敵管理クラス
	if (ImGui::TreeNode("EnemyManager")) {
		enemyManager_.Debug();
		ImGui::TreePop();
	}
	// 平面
	if (ImGui::TreeNode("Plane")) {
		plane.DebugGUI();
		ImGui::TreePop();
	}
	

	ImGui::End();

	// デバッグ用のカメラ
	mainCamera.DebugGUI();
#endif // _DEBUG
}
