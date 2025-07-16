#include "Title.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

void Title::Initialize() {
	//LWP::Window::ChangeFullScreenMode();

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

	//シーン切り替え機能生成
	sceneTransitioner_.Initialize(this);

	UIStart_.Initialize("Title/Start.png", "Title_Start");
	UIExit_.Initialize("Title/Exit.png", "Title_Exit");
	UITitleLogo_.Initialize("Title/Title_Logo.png", "Title_Logo");
	UITitleLogo_.isActive = false;

	selectUI_ = SelectUI::kStart;
}

void Title::Update() {
	
	///
	/// 突貫工事なので後で処理をまとめる
	///

	if (not sceneTransitioner_.GetIsSceneChange()) {

		if (selectUI_ == SelectUI::kStart) {

			//下キーでEndに移動
			if (Controller::GetTrigger(XBOX_DPAD_DOWN) || Keyboard::GetTrigger(DIK_DOWN)) {
				selectUI_ = SelectUI::kEnd;
			}
			//Aボタンでゲームスタート
			if (Controller::GetTrigger(XBOX_A) || Keyboard::GetTrigger(DIK_SPACE)) {
				sceneTransitioner_.SetNextScene(SceneName::kGameScene);
				sceneTransitioner_.SceneTransitionStart();
			}

			UIStart_.isActive = true;
			UIExit_.isActive = false;

		}
		else if (selectUI_ == SelectUI::kEnd) {

			//上キーでStartに移動
			if (Controller::GetTrigger(XBOX_DPAD_UP) || Keyboard::GetTrigger(DIK_UP)) {
				selectUI_ = SelectUI::kStart;
			}
			if (Controller::GetTrigger(XBOX_A) || Keyboard::GetTrigger(DIK_SPACE)) {
				LWP::System::ShutDown();
			}

			UIStart_.isActive = false;
			UIExit_.isActive = true;

		}

	}

	UIStart_.Update();
	UIExit_.Update();
	UITitleLogo_.Update();

	//SE管理
	sePlayer_.Update();
	//BGM管理
	bgmPlayer_.Update();

	sceneTransitioner_.Update();

	//デバッグ
	DebugGUI();

}

void Title::DebugGUI() {
#ifdef _DEBUG
	ImGui::Begin("DebugWindow");
	if (ImGui::BeginTabBar("GameObject")) {
		
		if (ImGui::BeginTabItem("Title")) {

			if (ImGui::TreeNode("Camera")) {
				mainCamera.DebugGUI();
				ImGui::TreePop();

			}

			ImGui::EndTabItem();

		}

		if (ImGui::BeginTabItem("UI")) {

			UIStart_.DebugGUI();
			UIExit_.DebugGUI();
			UITitleLogo_.DebugGUI();

			ImGui::EndTabItem();

		}

		ImGui::EndTabBar();
	}

	ImGui::End();
#endif // _DEBUG
}
