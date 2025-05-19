#include "HP.h"

HP::HP() {
	// HP画像の背景作成(BackGroundは長すぎるのでBGにしてます)
	sprite["HpBarBG"].LoadTexture("UI/Gauge/HpBarBackGround.png");
	sprite["HpBarBG"].isUI = true;
	sprite["HpBarBG"].isActive = true;
	// HP画像を作成
	sprite["HpBar"].LoadTexture("UI/Gauge/HpBar.png");
	sprite["HpBar"].anchorPoint = { 0.0f, 0.5f };
	sprite["HpBar"].worldTF.translation = { LWP::Info::GetWindowWidthF() / 2, 20.0f, 0.0f };
	sprite["HpBar"].isUI = true;
	sprite["HpBar"].isActive = true;
	maxSize_ = sprite["HpBar"].size;

	// jsonに保存する値を設定
	json_.Init("HP.json");
	json_.BeginGroup("WorldTransform")
		.AddValue<LWP::Math::Vector3>("Translation", &sprite["HpBar"].worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &sprite["HpBar"].worldTF.scale)
		.EndGroup()
		.AddValue<LWP::Math::Vector2>("AnchorPoint", &sprite["HpBar"].anchorPoint)
		.AddValue<float>("MaxHpValue", &maxValue)
		.AddValue<float>("DeltaValue", &deltaValue)
		.AddValue<float>("Multiply", &multiply)
		.CheckJsonFile();

	// HPを最大値にする
	value = maxValue;

	// HPバーと値をそろえる
	sprite["HpBarBG"].anchorPoint = sprite["HpBar"].anchorPoint;
	sprite["HpBarBG"].worldTF.translation = sprite["HpBar"].worldTF.translation;
}

void HP::Initialize() {

}

void HP::Update() {
	// HPが0よりも下にならないようにする
	if (GetValueEmpty()) { value = 0.0f; }

	// HPバーの長さ計算
	ColGaugeSize("HpBar");

	// ゲージの背景の座標を代入
	//sprite["HpBarBG"].worldTF.translation = sprite["HpBar"].worldTF.translation;
}

void HP::DebugGUI() {
	if (ImGui::TreeNode("HP")) {
		// 保存する値
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			ImGui::TreePop();
		}
		// HPバー
		if (ImGui::TreeNode("Sprite")) {
			sprite["HpBar"].DebugGUI();
			ImGui::DragFloat3("HpBarBackGround", &sprite["HpBarBG"].worldTF.translation.x, 0.01f);
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