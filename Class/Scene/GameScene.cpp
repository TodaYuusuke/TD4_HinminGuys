#include "GameScene.h"
#include "Title.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

// 初期化
void GameScene::Initialize() {
	// 自機の動作確認のため生成
	player_ = std::make_unique<Player>();
	player_->Initialize();

	// 追従カメラの動作確認のため生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTargetPos(player_->GetModelPos());
	followCamera_->SetCamera(&mainCamera);
	//followCamera_->SetTarget(&player_->GetWorldTF());

	// 一時的に平面を生成
	plane.LoadShortPath("field/ground/SimpleStage.gltf");
	plane.worldTF.translation = { 0,-5,0 };
}

// 更新
void GameScene::Update() {
	// シーン遷移
	if (Keyboard::GetTrigger(DIK_P)) {
		nextSceneFunction = []() { return new Title(); };
	}

	// 自機
	player_->Update();

	//followCamera_->Update();

	mainCamera.DebugGUI();
}