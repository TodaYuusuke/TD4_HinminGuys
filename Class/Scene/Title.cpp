#include "Title.h"
#include "GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

void Title::Initialize() {
	tree_.Init();
}

void Title::Update() {

	tree_.Update();
	tree_.DebugGUI();

	// シーン遷移
	/*if (Keyboard::GetTrigger(DIK_P)) {
		nextSceneFunction = []() { return new GameScene(); };
	}*/
}
