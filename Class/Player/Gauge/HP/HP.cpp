#include "HP.h"

HP::HP() {
	// HP画像の背景作成(BackGroundは長すぎるのでBGにしてます)
	sprite_["HpBarBG"].LoadTexture("UI/Gauge/HpBarBackGround.png");
	sprite_["HpBarBG"].Init();
	sprite_["HpBarBG"].isUI = true;
	sprite_["HpBarBG"].isActive = true;
	// HP画像を作成
	sprite_["HpBar"].LoadTexture("UI/Gauge/HpBar.png");
	sprite_["HpBar"].Init();
	sprite_["HpBar"].anchorPoint = { 0.0f, 0.5f };
	sprite_["HpBar"].worldTF.translation = { LWP::Info::GetWindowWidthF() / 2, 20.0f, 0.0f };
	sprite_["HpBar"].isUI = true;
	sprite_["HpBar"].isActive = true;
	maxSize_ = sprite_["HpBar"].size;

	// jsonに保存する値を設定
	json_.Init("HP.json");
	json_.BeginGroup("WorldTransform")
		.AddValue<LWP::Math::Vector3>("Translation", &sprite_["HpBar"].worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &sprite_["HpBar"].worldTF.scale)
		.EndGroup()
		.AddValue<LWP::Math::Vector2>("AnchorPoint", &sprite_["HpBar"].anchorPoint)
		.AddValue<float>("MaxHpValue", &maxValue_)
		.AddValue<float>("DeltaValue", &deltaValue_)
		.AddValue<float>("Multiply", &multiply_)
		.CheckJsonFile();

	// HPを最大値にする
	value_ = maxValue_;

	// HPバーと値をそろえる
	sprite_["HpBarBG"].anchorPoint = sprite_["HpBar"].anchorPoint;
	sprite_["HpBarBG"].worldTF.translation = sprite_["HpBar"].worldTF.translation;
}

void HP::Initialize() {

}

void HP::Update() {
	// HPが0よりも下にならないようにする
	if (GetValueEmpty()) { value_ = 0.0f; }

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
			sprite_["HpBar"].DebugGUI();
			ImGui::DragFloat3("HpBarBackGround", &sprite_["HpBarBG"].worldTF.translation.x, 0.01f);
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