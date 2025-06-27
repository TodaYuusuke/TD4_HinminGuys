#include "GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

GameScene::GameScene()
	: player_(&mainCamera, &enemyManager_, &followCamera_, &uiManager_),
	followCamera_(&player_, &mainCamera, player_.GetModelPos()),
	uiManager_(&player_)
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

	// ヒットストップの管理クラス
	hitStopController_ = HitStopController::GetInstance();
	hitStopController_->Initialize();

	//レベルロードッツ！！
	levelData.LoadShortPath("gameScene.json");

	// UIの管理クラスを生成
	uiManager_.Initialize();

	// 敵管理クラス
	enemyManager_.Initialize();
	enemyManager_.SetPlayer(&player_);
	enemyManager_.SetDamageEffectEmitter(&damageEffectEmitter_);
	enemyManager_.SetCamera(followCamera_.GetCamera());
	enemyManager_.SetIsShowSpawnDataModel(false);

	// 追従カメラの動作確認のため生成
	followCamera_.Initialize();

	// 自機の動作確認のため生成
	player_.Initialize();

	//ダメージエフェクトエミッターを生成
	damageEffectEmitter_.Initialize();
	damageEffectEmitter_.SetCamera(followCamera_.GetCamera());

#pragma region フィールドを一時的に生成
	// 一時的に平面を生成
	plane.LoadShortPath("field/ground/SimpleStage.gltf");
	plane.worldTF.scale = { 1000.0f,1000.0f ,1000.0f };
	plane.worldTF.translation = { 0,0,0 };
	plane.materials["Material"].uvTransform.scale = { 10000.0f,10000.0f ,0.0f };
	// 一時的に天球を生成
	skydome.LoadShortPath("field/skydome/SkyDome.gltf");
	skydome.worldTF.scale = { 1000.0f,1000.0f ,1000.0f };
	skydome.materials["SkyDomeMaterial"].uvTransform.scale = { 20.0f,10.0f ,1.0f };
	skydome.SetAllMaterialLighting(false);
#pragma endregion

	// 平行光源を配置(これも一時的に配置)
	light_.worldTF.translation = { 0,10,0 };

	//シーン切り替え機能生成
	sceneTransitioner_.Initialize(this);
}

// 更新
void GameScene::Update() {
	
	//シーン遷移が終わった時点でウェーブを開始していない場合、ウェーブを開始
	if (not sceneTransitioner_.GetIsSceneChange() and not enemyManager_.GetIsDefeatedAllEnemy() and
		not enemyManager_.GetIsStartWave()) {

#ifdef _DEBUG

#else
		enemyManager_.StartWave();
#endif // _DEBUG

	}

	//全ての敵が倒されたらシーン遷移する
	if (enemyManager_.GetIsDefeatedAllEnemy()) {
		//遷移先をタイトルにセット
		sceneTransitioner_.SetNextScene(SceneName::kTitle);
		sceneTransitioner_.SceneTransitionStart();
	}
	//全員倒す前にプレイヤーが死んだ場合、ゲームオーバーに逝こう！
	else if (not enemyManager_.GetIsDefeatedAllEnemy() and not player_.GetIsAlive()) {
		//遷移先をゲームオーバーにセット
		sceneTransitioner_.SetNextScene(SceneName::kGameOver);
		sceneTransitioner_.SceneTransitionStart();
	}

	// 入力されたコマンドを確認
	inputHandler_->Update(player_);

	// ヒットストップ
	hitStopController_->Update();

	//敵全て
	enemyManager_.Update();

	// 追従カメラ
	followCamera_.Update();

	// 自機
	player_.Update();

	//ダメージエフェクトエミッター
	damageEffectEmitter_.Update();

	// uiの管理クラス
	uiManager_.Update();

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
		//ダメージエフェクトエミッター
		if (ImGui::BeginTabItem("DamageEffectEmitter")) {
			damageEffectEmitter_.DebugGUI();
			ImGui::EndTabItem();
		}
		// 地面
		if (ImGui::BeginTabItem("Ground")) {
			plane.DebugGUI();
			ImGui::EndTabItem();
		}
		// UI管理クラス
		if (ImGui::BeginTabItem("UI")) {
			uiManager_.DebugGUI();
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
