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
	: player_(&mainCamera, &enemyManager_),
	  followCamera_(&mainCamera, player_.GetModelPos())
{
	enemyManager_.Initialize();
}

GameScene::~GameScene()
{
	enemyManager_.Finalize();
}

GameScene::~GameScene() {
	enemyManager_.Finalize();
}

// 初期化
void GameScene::Initialize() {

	enemyManager_.Initialize();
	enemyManager_.SetPlayer(&player_);

	// 追従カメラの動作確認のため生成
	followCamera_.Initialize();

	// 自機の動作確認のため生成
	player_.Initialize();

	// 一時的に平面を生成
	plane.LoadShortPath("field/ground/SimpleStage.gltf");
	plane.worldTF.translation = { 0,-5,0 };

	light_.worldTF.translation = { 0,10,0 };
}

// 更新
void GameScene::Update() {
	// シーン遷移
	if (Keyboard::GetTrigger(DIK_P)) {
		nextSceneFunction = []() { return new Title(); };
	}

#ifdef _DEBUG

	enemyManager_.Debug();

#endif // _DEBUG


	// 自機
	player_.Update();
	//敵全て
	enemyManager_.Update();

	followCamera_.Update();

	mainCamera.DebugGUI();
}