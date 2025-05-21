#include "Title.h"
#include "GameScene.h"
#include "VoidScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

void VoidScene::Initialize() {

	//シーン切り替え機能生成
	sceneTransitioner_.Initialize(this);

}

void VoidScene::Update() {
	
	sceneTransitioner_.Update();

}
