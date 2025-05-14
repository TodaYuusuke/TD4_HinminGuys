#include "SheathGauge.h"

SheathGauge::SheathGauge() {
	// HP画像の背景作成(BackGroundは長すぎるのでBGにしてます)
	sprite["SheathBarBG"].LoadTexture("UI/Gauge/SheathBarBackGround.png");
	sprite["SheathBarBG"].isUI = true;
	sprite["SheathBarBG"].isActive = true;
	// HP画像を作成
	sprite["SheathBar"].LoadTexture("UI/Gauge/SheathBar.png");
	sprite["SheathBar"].anchorPoint = { 0.0f, 0.5f };
	sprite["SheathBar"].worldTF.translation = { LWP::Info::GetWindowWidthF() / 2, 20.0f, 0.0f };
	sprite["SheathBar"].isUI = true;
	sprite["SheathBar"].isActive = true;
	maxSize_ = sprite["SheathBar"].size;

	// jsonに保存する値を設定
	json_.Init("SheathGauge.json");
	json_.BeginGroup("WorldTransform")
		.AddValue<LWP::Math::Vector3>("Translation", &sprite["SheathBar"].worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &sprite["SheathBar"].worldTF.scale)
		.EndGroup()
		.AddValue<LWP::Math::Vector2>("AnchorPoint", &sprite["SheathBar"].anchorPoint)
		.AddValue<float>("MaxHpValue", &maxValue)
		.AddValue<float>("DeltaValue", &deltaValue)
		.AddValue<float>("Multiply", &multiply)
		.CheckJsonFile();

	// HPを最大値にする
	value = maxValue;

	// HPバーと値をそろえる
	sprite["SheathBarBG"].anchorPoint = sprite["SheathBar"].anchorPoint;
	sprite["SheathBarBG"].worldTF.translation = sprite["SheathBar"].worldTF.translation;
}

void SheathGauge::Initialize() {

}

void SheathGauge::Update() {
	// HPが0よりも下にならないようにする
	if (GetValueEmpty()) { value = 0.0f; }

	// HPバーの長さ計算
	ColGaugeSize("SheathBar");

	// ゲージの背景の座標を代入
	sprite["SheathBarBG"].worldTF.translation = sprite["SheathBar"].worldTF.translation;
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
			sprite["SheathBar"].DebugGUI();
			ImGui::TreePop();
		}
		ImGui::DragFloat("Value", &value);
		ImGui::DragFloat("MaxValue", &maxValue);
		ImGui::DragFloat("DeltaValue", &deltaValue);
		ImGui::DragFloat("Multiply", &multiply);
		ImGui::Checkbox("IsHit", &isHit);
		ImGui::TreePop();
	}
}