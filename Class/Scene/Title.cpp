#include "Title.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

void Title::Initialize() {

#ifndef _DEBUG
	// リリースではフルスクリーンに
	LWP::Window::ChangeFullScreenMode();
#endif // !_DEBUG

	// 平行光源を調整
	LWP::Object::Manager::GetInstance()->GetDirLight()->worldTF.rotation = { 0.8f, 0.0f, 0.0f, 0.6f };

	//タイトルで事前読み込み
	LWP::Resource::LoadModel("resources/model/Saiji/Saiji_IK.gltf");
	LWP::Resource::LoadModel("resources/model/Oniheihe/Oniheihe_IK.gltf");
	LWP::Resource::LoadModel("resources/model/Ogre/Orga_IK.gltf");
	LWP::Resource::LoadModel("resources/model/Saiji/Club.gltf");
	LWP::Resource::LoadModel("resources/model/Oniheihe/MatchLockGun.gltf");
	LWP::Resource::LoadModel("resources/model/Ogre/Weapon.gltf");

	//レベルロードッツ！！
	levelData.LoadShortPath("gameScene.json");

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

	// カメラ座標の設定
	mainCamera.worldTF.translation = { 0.0f, 1.0f, -10.0f };

	// プレイヤーモデル生成
	playerModel_.LoadShortPath("player/Player.gltf");
	playerAnimation_.LoadFullPath("resources/model/player/Player.gltf", &playerModel_);
	playerAnimation_.Play("Walk");
	playerAnimation_.Loop();

	// 刀
	swordModel_.LoadShortPath("player/Katana.gltf");
	// 鞘
	sheathModel_.LoadShortPath("player/Sheath.gltf");
	// 刀モデルをプレイヤーの手に追従させる
	swordModel_.GetJoint("Grip")->localTF.Parent(&playerModel_, "WeaponAnchor");
	// 鞘モデルを刀モデルに追従
	sheathModel_.GetJoint("Sheath")->localTF.Parent(&swordModel_, "Sheath");

	// プレイヤーモデルをカメラと親子付け
	playerModel_.worldTF.Parent(&mainCamera.worldTF);
	// プレイヤーの座標設定
	playerModel_.worldTF.translation = { -2.0f, -1.0f, 6.5f };
	playerModel_.worldTF.rotation = { -0.0f, 0.703f, 0.0f, 0.711f };

	//シーン切り替え機能生成
	sceneTransitioner_.Initialize(this);

	UIStart_.Initialize("Title/Start.png", "Title_Start");
	UIExit_.Initialize("Title/Exit.png", "Title_Exit");
	UITitleLogo_.Initialize("Title/Title_Logo.png", "Title_Logo");
	UITitleLogo_.isActive = false;

	selectUI_ = SelectUI::kStart;

	// BGMの再生
	if (bgmPath_ != "") {
		bgmPlayer_.PlayBGM(bgmPath_, "TitleBGM", 1.0f);
	}
}

void Title::Update() {
	
	///
	/// 突貫工事なので後で処理をまとめる
	///

	// カメラを徐々に回転させる
	mainCamera.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, 0.003f) * mainCamera.worldTF.rotation;

	if (not sceneTransitioner_.GetIsSceneChange()) {

		if (selectUI_ == SelectUI::kStart) {

			//下キーでEndに移動
			if (Controller::GetTrigger(XBOX_DPAD_DOWN) || Keyboard::GetTrigger(DIK_DOWN)) {
				// カーソル移動音を鳴らす
				sePlayer_.PlaySE(moveSound_, "Move", seVolume_);
				selectUI_ = SelectUI::kEnd;
			}
			//Aボタンでゲームスタート
			if (Controller::GetTrigger(XBOX_A) || Keyboard::GetTrigger(DIK_SPACE)) {
				// BGMの停止
				bgmPlayer_.Stop("TitleBGM");
				// 決定音を鳴らす
				sePlayer_.PlaySE(decideSound_, "Decide", seVolume_);
				sceneTransitioner_.SetNextScene(SceneName::kGameScene);
				sceneTransitioner_.SceneTransitionStart();
			}

			UIStart_.isActive = true;
			UIExit_.isActive = false;

		}
		else if (selectUI_ == SelectUI::kEnd) {

			//上キーでStartに移動
			if (Controller::GetTrigger(XBOX_DPAD_UP) || Keyboard::GetTrigger(DIK_UP)) {
				// カーソル移動音を鳴らす
				sePlayer_.PlaySE(moveSound_, "Move", seVolume_);
				selectUI_ = SelectUI::kStart;
			}
			if (Controller::GetTrigger(XBOX_A) || Keyboard::GetTrigger(DIK_SPACE)) {
				// 終了音を鳴らす
				sePlayer_.PlaySE(endSound_, "End", seVolume_);
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
