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
void GameScene::Initialize() {}

// 更新
void GameScene::Update() {
	// シーン遷移
	if (Keyboard::GetTrigger(DIK_P)) {
		nextSceneFunction = []() { return new Title(); };
	}
}