#include "Title.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

void Title::Initialize() {

	//刀モデル読み込み
	sword_.LoadShortPath("player/SimpleWeapon.gltf");
	sword_.worldTF.rotation = LWP::Math::Quaternion::ConvertEuler(LWP::Math::Vector3(0.0f, 0.0f, 3.14f));

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

}

void Title::Update() {
	// シーン遷移
	if (Keyboard::GetTrigger(DIK_P)) {
		//遷移先をタイトルにセット
		sceneTransitioner_.SetNextScene(SceneName::kGameScene);
		sceneTransitioner_.SceneTransitionStart();
	}

	if (Keyboard::GetTrigger(DIK_SPACE)) {
		UIStart_.isActive = not UIStart_.isActive;
		UIExit_.isActive = not UIExit_.isActive;
		UITitleLogo_.isActive = not UITitleLogo_.isActive;
	}

	UIStart_.Update();
	UIExit_.Update();
	UITitleLogo_.Update();

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
