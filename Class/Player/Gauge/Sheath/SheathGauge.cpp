#include "SheathGauge.h"

SheathGauge::SheathGauge() {
	// HP画像の背景作成(BackGroundは長すぎるのでBGにしてます)
	sprite_["SheathBarBG"].LoadTexture("UI/Gauge/SheathBarBackGround.png");
	sprite_["SheathBarBG"].Init();
	sprite_["SheathBarBG"].isUI = true;
	sprite_["SheathBarBG"].isActive = true;
	// HP画像を作成
	sprite_["SheathBar"].LoadTexture("UI/Gauge/SheathBar.png");
	sprite_["SheathBar"].Init();
	sprite_["SheathBar"].isUI = true;
	sprite_["SheathBar"].isActive = true;

	// jsonに保存する値を設定
	json_.Init("SheathGauge.json");
	json_.BeginGroup("Gauge")
		.BeginGroup("WorldTransform")
		.AddValue<LWP::Math::Vector3>("Translation", &sprite_["SheathBar"].worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &sprite_["SheathBar"].worldTF.scale)
		.EndGroup()
		.AddValue<LWP::Math::Vector2>("AnchorPoint", &sprite_["SheathBar"].anchorPoint)
		.EndGroup()
		
		.BeginGroup("GaugeBackGround")
		.BeginGroup("WorldTransform")
		.AddValue<LWP::Math::Vector3>("Translation", &sprite_["SheathBarBG"].worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &sprite_["SheathBarBG"].worldTF.scale)
		.EndGroup()
		.AddValue<LWP::Math::Vector2>("AnchorPoint", &sprite_["SheathBarBG"].anchorPoint)
		.EndGroup()

		.AddValue<float>("MaxHpValue", &maxValue_)
		.AddValue<float>("DeltaValue", &deltaValue_)
		.AddValue<float>("Multiply", &multiply_)
		.CheckJsonFile();

	maxSize_ = { 1.0f, 1.0f };
	// HPを最大値にする
	value_ = maxValue_;
}

void SheathGauge::Initialize() {

}

void SheathGauge::Update() {
	// HPが0よりも下にならないようにする
	if (GetValueEmpty()) { value_ = 0.0f; }

	// HPバーの長さ計算
	ColGaugeSize("SheathBar");
}

void SheathGauge::DebugGUI() {
	if (ImGui::TreeNode("SheathGauge")) {
		// 保存する値
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			ImGui::TreePop();
		}
		// HPバー
		if (ImGui::TreeNode("Sprite")) {
			sprite_["SheathBar"].DebugGUI();
			ImGui::TreePop();
		}
		ImGui::DragFloat("Value", &value_);
		ImGui::DragFloat("MaxValue", &maxValue_);
		ImGui::DragFloat("DeltaValue", &deltaValue_);
		ImGui::DragFloat("Multiply", &multiply_);
		ImGui::Checkbox("IsHit", &isHit_);
		ImGui::TreePop();
	}
}