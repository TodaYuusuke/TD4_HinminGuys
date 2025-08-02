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
	uiManager_(&player_),
	particles_(&player_, &followCamera_),
	gameSceneManager_(&player_, &enemyManager_, &sceneTransitioner_, &bgmPlayer_, &sePlayer_)
{
	enemyManager_.Initialize();
	LWP::Resource::LoadTexture("Effect/Particle.png");
	LWP::Resource::LoadTexture("Effect/Spark.png");
	LWP::Resource::LoadTexture("Effect/SwordSlash.png");
	LWP::Resource::LoadTexture("Effect/CircleParticle.png");
	LWP::Resource::LoadTexture("Effect/ParryFlash.png");
	LWP::Resource::LoadTexture("Effect/Rock.png");
	LWP::Resource::LoadTexture("Effect/Smoke.png");
	LWP::Resource::LoadTexture("Effect/Crack.png");
	LWP::Resource::LoadTexture("UI/ButtonUI/LockOn.png");
	LWP::Resource::LoadTexture("lockOnReticle.png");
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

	// パーティクルの管理クラス
	particles_.Initialize();

	// 敵管理クラス
	enemyManager_.Initialize();
	enemyManager_.SetPlayer(&player_);
	enemyManager_.SetDamageEffectEmitter(&damageEffectEmitter_);
	enemyManager_.SetCamera(followCamera_.GetCamera());
	enemyManager_.SetIsShowSpawnDataModel(false);
	enemyManager_.SetSEPlayer(&sePlayer_);
	enemyManager_.SetWorld(&world_);

	// 追従カメラの生成
	followCamera_.Initialize();

	// 自機の生成
	player_.SetSEPlayer(&sePlayer_);
	player_.Initialize();
	player_.SetParticles(&particles_);
	player_.SetWorld(&world_);

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

	// ゲームシーンマネージャーの初期化
	gameSceneManager_.Init();

	// BGM再生
	bgmPlayer_.PlayBGM("BattleBGM.mp3", "BattleBGM", 1.0f);
}

// 更新
void GameScene::Update() {
	
	// ゲームシーンマネージャの更新
	gameSceneManager_.Update();

	//SE管理
	sePlayer_.Update();
	//BGM管理
	bgmPlayer_.Update();

	// ゲームが終了状態の場合
	if (gameSceneManager_.GetIsEndGame()) {
		#ifdef _DEBUG
		// デバッグ用のウィンドウ
		DebugGUI();
		#endif // _DEBUG
		// 早期リターン
		return;
	}

	//シーン遷移が終わった時点でウェーブを開始していない場合、ウェーブを開始
	if (not sceneTransitioner_.GetIsSceneChange() and not enemyManager_.GetIsDefeatedAllEnemy() and
		not enemyManager_.GetIsStartWave()) {

#ifdef _DEBUG

#else
		enemyManager_.StartWave();
#endif // _DEBUG

	}

	// 入力されたコマンドを確認
	inputHandler_->Update(player_);

	// ヒットストップ
	hitStopController_->Update();

	//敵全て
	enemyManager_.Update();

	// 自機
	player_.Update();

	// 追従カメラ
	followCamera_.Update();
	
	//ダメージエフェクトエミッター
	damageEffectEmitter_.Update();

	// パーティクル管理クラス
	particles_.Update();

	// uiの管理クラス
	uiManager_.Update();

	// デバッグ用のウィンドウ
	DebugGUI();
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
		// パーティクル管理クラス
		if (ImGui::BeginTabItem("Particles")) {
			particles_.DebugGui();
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
		// 世界
		if (ImGui::BeginTabItem("World")) {
			world_.DebugGUI();
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

			if (ImGui::Button("Level Reload")) {
				levelData.HotReload();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

	// コンボのデバッグGUIの表示
	player_.DebugComboGUI();

#endif // _DEBUG
}
