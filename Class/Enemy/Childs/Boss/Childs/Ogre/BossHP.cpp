#include "BossHP.h"
#include "../../../../../Player/Math/MathFunctions.h"

BossHP::BossHP() {
	// HP画像の背景作成(BackGroundは長すぎるのでBGにしてます)
	sprite_["BossHpBarBG"].LoadTexture("UI/Gauge/BossHpBarBase.png");
	sprite_["BossHpBarBG"].isActive = true;
	// HP画像を作成
	sprite_["BossHpBarDecriment"].LoadTexture("UI/Gauge/BossHpBarGauge.png");
	sprite_["BossHpBarDecriment"].isActive = true;
	sprite_["BossHpBarDecriment"].material.color = { 50,50,50,255 };
	// HP画像を作成
	sprite_["BossHpBar"].LoadTexture("UI/Gauge/BossHpBarGauge.png");
	sprite_["BossHpBar"].isActive = true;

	// jsonに保存する値を設定
	json_.Init("BossHP.json");
	// HPバー
	json_.BeginGroup("Gauge")
		.BeginGroup("WorldTransform")
		.AddValue<LWP::Math::Vector3>("Translation", &sprite_["BossHpBar"].worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &sprite_["BossHpBar"].worldTF.scale)
		.EndGroup()
		.AddValue<LWP::Math::Vector2>("AnchorPoint", &sprite_["BossHpBar"].anchorPoint)
		.EndGroup()
		// HPバーの背景
		.BeginGroup("GaugeBackGround")
		.BeginGroup("WorldTransform")
		.AddValue<LWP::Math::Vector3>("Translation", &sprite_["BossHpBarBG"].worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &sprite_["BossHpBarBG"].worldTF.scale)
		.EndGroup()
		.AddValue<LWP::Math::Vector2>("AnchorPoint", &sprite_["BossHpBarBG"].anchorPoint)
		.EndGroup()

		.AddValue<float>("MaxHpValue", &maxValue_)
		.AddValue<float>("DeltaValue", &deltaValue_)
		.AddValue<float>("Multiply", &multiply_)
		.CheckJsonFile();

	maxSize_ = sprite_["BossHpBar"].material.texture.t.GetSize();
	sprite_["BossHpBar"].clipRect.max = maxSize_;
	sprite_["BossHpBarDecriment"].clipRect.max = maxSize_;
	sprite_["BossHpBarBG"].clipRect.max = sprite_["BossHpBarBG"].material.texture.t.GetSize();

	sprite_["BossHpBarDecriment"].worldTF = sprite_["BossHpBar"].worldTF;
	sprite_["BossHpBarDecriment"].anchorPoint = sprite_["BossHpBar"].anchorPoint;
	// HPを最大値にする
	value_ = maxValue_;
	multiply_ = 1.0f;
}

void BossHP::Initialize() {

}

void BossHP::Update() {
	// HPが0よりも下にならないようにする
	if (GetValueEmpty()) { value_ = 0.0f; }

	// HPバーの長さ計算
	ColGaugeSize("BossHpBar");

	// ゲージ増加中はしない
	sprite_["BossHpBarDecriment"].clipRect.max.x = MathFunc::ExponentialInterpolateF(sprite_["BossHpBarDecriment"].clipRect.max.x, sprite_["BossHpBar"].clipRect.max.x, 0.05f);
}

void BossHP::DebugGUI() {
	if (ImGui::TreeNode("BossBar")) {
		// 保存する値
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			ImGui::TreePop();
		}
		// HPバー
		if (ImGui::TreeNode("Sprite")) {
			sprite_["BossHpBar"].DebugGUI();
			ImGui::DragFloat3("HpBarBackGround", &sprite_["BossHpBarBG"].worldTF.translation.x, 0.01f);
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