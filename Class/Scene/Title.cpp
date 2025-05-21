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

	// 平行光源を配置(これも一時的に配置)
	light_.worldTF.translation = { 0,10,0 };

	//シーン切り替え機能生成
	sceneTransitioner_.Initialize(this);

}

void Title::Update() {
	// シーン遷移
	if (Keyboard::GetTrigger(DIK_P)) {
		//遷移先をタイトルにセット
		sceneTransitioner_.SetNextScene(SceneName::kGameScene);
		sceneTransitioner_.SceneTransitionStart();
	}


	sceneTransitioner_.Update();

}
