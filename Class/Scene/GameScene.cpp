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
	: player_(&mainCamera),
	  followCamera_(&mainCamera, player_.GetModelPos())
{}

// 初期化
void GameScene::Initialize() {

	// 追従カメラの動作確認のため生成
	followCamera_.Initialize();

	// 自機の動作確認のため生成
	player_.Initialize();

	// 一時的に平面を生成
	plane.LoadShortPath("field/ground/SimpleStage.gltf");
	plane.worldTF.translation = { 0,-5,0 };

	directionLight_.worldTF.translation = { 0,10,0 };
}

// 更新
void GameScene::Update() {
	// シーン遷移
	if (Keyboard::GetTrigger(DIK_P)) {
		nextSceneFunction = []() { return new Title(); };
	}

	// 自機
	player_.Update();

	followCamera_.Update();

	mainCamera.DebugGUI();
}