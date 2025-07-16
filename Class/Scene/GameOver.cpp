#include "GameOver.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

void GameOver::Initialize() {

	// 平行光源を配置(これも一時的に配置)
	light_.worldTF.translation = { 0,10,0 };

#pragma region フィールドを一時的に生成
	// 一時的に平面を生成
	plane_.LoadShortPath("field/ground/SimpleStage.gltf");
	plane_.worldTF.scale = { 1000.0f,1000.0f ,1000.0f };
	plane_.worldTF.translation = { 0,0,0 };
	plane_.materials["Material"].uvTransform.scale = { 1000.0f,1000.0f ,0.0f };
	// 一時的に天球を生成
	skydome_.LoadShortPath("field/skydome/SkyDome.gltf");
	skydome_.worldTF.scale = { 1000.0f,1000.0f ,1000.0f };
	skydome_.SetAllMaterialLighting(false);
#pragma endregion

	UIGameOver_.Initialize("GameOver/GameOver.png", "GameOver");
	UI_Button_A_.Initialize("UI/Button_A.png", "Button_A");

	//シーン切り替え機能生成
	sceneTransitioner_.Initialize(this);

}

void GameOver::Update() {

	///
	/// 突貫工事なので後で処理をまとめる
	///

	if (not sceneTransitioner_.GetIsSceneChange()) {

		//Aボタンでタイトルへ
		if (Controller::GetTrigger(XBOX_A)) {
			sceneTransitioner_.SetNextScene(SceneName::kTitle);
			sceneTransitioner_.SceneTransitionStart();
		}

	}

	sceneTransitioner_.Update();

	UIGameOver_.Update();
	UI_Button_A_.Update();

	//SE管理
	sePlayer_.Update();
	//BGM管理
	bgmPlayer_.Update();

	//デバッグ
	DebugGUI();

}

void GameOver::DebugGUI() {
#ifdef _DEBUG
	ImGui::Begin("DebugWindow");
	if (ImGui::BeginTabBar("GameObject")) {

		if (ImGui::BeginTabItem("GameOver")) {

			if (ImGui::TreeNode("Camera")) {
				mainCamera.DebugGUI();
				ImGui::TreePop();

			}

			ImGui::EndTabItem();

		}

		if (ImGui::BeginTabItem("UI")) {

			UIGameOver_.DebugGUI();
			UI_Button_A_.DebugGUI();

			ImGui::EndTabItem();

		}

		ImGui::EndTabBar();
	}

	ImGui::End();
#endif // _DEBUG
}
