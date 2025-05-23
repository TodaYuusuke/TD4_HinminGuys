#include "GameScene.h"

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
	inputHandler_ = InputHandler::GetInstance();
	inputHandler_->Initialize();

	// 敵管理クラス
	enemyManager_.Initialize();
	enemyManager_.SetPlayer(&player_);

	// 追従カメラの動作確認のため生成
	followCamera_.Initialize();

	// 自機の動作確認のため生成
	//player_.SetInputHandler(inputHandler_);
	player_.Initialize();

#pragma region フィールドを一時的に生成
	// 一時的に平面を生成
	plane.LoadShortPath("field/ground/SimpleStage.gltf");
	plane.worldTF.scale = { 1000.0f,1000.0f ,1000.0f };
	plane.worldTF.translation = { 0,0,0 };
	plane.materials["Material"].uvTransform.scale = { 1000.0f,1000.0f ,0.0f };
	// 一時的に天球を生成
	skydome.LoadShortPath("field/skydome/SkyDome.gltf");
	skydome.worldTF.scale = { 1000.0f,1000.0f ,1000.0f };
	skydome.SetAllMaterialLighting(false);
#pragma endregion

	// 平行光源を配置(これも一時的に配置)
	light_.worldTF.translation = { 0,10,0 };

	//シーン切り替え機能生成
	sceneTransitioner_.Initialize(this);

}

// 更新
void GameScene::Update() {
	// シーン遷移
	if (Keyboard::GetTrigger(DIK_P)) {
		//遷移先をタイトルにセット
		sceneTransitioner_.SetNextScene(SceneName::kTitle);
		sceneTransitioner_.SceneTransitionStart();
	}

	// 入力されたコマンドを確認
	inputHandler_->Update(player_);

		//敵全て
		enemyManager_.Update();

		// 追従カメラ
		followCamera_.Update();

		// 自機
		player_.Update();

		// デバッグ用のウィンドウ
		DebugGUI();

	sceneTransitioner_.Update();

}

void GameScene::DebugGUI() {
#ifdef _DEBUG
	ImGui::Begin("DebugWindow");
	if (ImGui::BeginTabBar("GameObject")) {		
		// 自機
		if (ImGui::BeginTabItem("Player")) {
			player_.DebugGUI();
			ImGui::EndTabItem();
		}
		// キーコンフィグ
		if (ImGui::BeginTabItem("InputHandler")) {
			inputHandler_->DebugGUI();
			ImGui::EndTabItem();
		}
		// 追従カメラ
		if (ImGui::BeginTabItem("FollowCamera")) {
			followCamera_.DebugGUI();
			ImGui::EndTabItem();
		}
		// 敵管理クラス
		if (ImGui::BeginTabItem("EnemyManager")) {
			enemyManager_.DebugGUI();
			ImGui::EndTabItem();
		}
		// 地面
		if (ImGui::BeginTabItem("Ground")) {
			plane.DebugGUI();
			ImGui::EndTabItem();
		}
		// デバッグ用のカメラ
		if (ImGui::BeginTabItem("DebugCamera")) {
			mainCamera.DebugGUI();
			ImGui::EndTabItem();
		}
		// FPSカウンターの表示
		if (ImGui::BeginTabItem("Other")) {
			ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

	// コンボのデバッグGUIの表示
	player_.DebugComboGUI();

#endif // _DEBUG
}
