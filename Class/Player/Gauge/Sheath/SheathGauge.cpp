#include "SheathGauge.h"
#include "../../Math/MathFunctions.h"

SheathGauge::SheathGauge() {
	// 鞘ゲージ画像の背景作成(BackGroundは長すぎるのでBGにしてます)
	sprite_["SheathBarBG"].LoadTexture("UI/Gauge/SheathBar/SheathBarBase.png");
	sprite_["SheathBarBG"].isActive = true;
	// ゲージ減少時の後追いするゲージ
	sprite_["SheathBarDecriment"].LoadTexture("UI/Gauge/SheathBar/SheathBarGauge.png");
	sprite_["SheathBarDecriment"].isActive = true;
	sprite_["SheathBarDecriment"].material.color = { 50,50,50,255 };
	// 鞘ゲージ画像を作成
	sprite_["SheathBar"].LoadTexture("UI/Gauge/SheathBar/SheathBarGauge.png");
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
		.AddValue<float>("IncreaseTime", &increaseTime)
		.CheckJsonFile();

	maxSize_ = sprite_["SheathBar"].material.texture.t.GetSize();
	sprite_["SheathBar"].clipRect.max = maxSize_;
	sprite_["SheathBarDecriment"].clipRect.max = maxSize_;
	sprite_["SheathBarBG"].clipRect.max = maxSize_;

	sprite_["SheathBarDecriment"].worldTF = sprite_["SheathBar"].worldTF;
	sprite_["SheathBarDecriment"].anchorPoint = sprite_["SheathBar"].anchorPoint;
	// HPを最大値にする
	value_ = maxValue_;
}

void SheathGauge::Initialize() {

}

void SheathGauge::Update() {
	// ゲージの増加
	IncreaseGauge();

	// HPの最大値最小値制限
	value_ = std::clamp<float>(value_, 0.0f, maxValue_);

	// HPバーの長さ計算
	ColGaugeSize("SheathBar");

	// ゲージ増加中はしない
	if (!isIncrease_) {
		sprite_["SheathBarDecriment"].clipRect.max.x = MathFunc::ExponentialInterpolateF(sprite_["SheathBarDecriment"].clipRect.max.x, sprite_["SheathBar"].clipRect.max.x, 0.05f);
	}
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

void SheathGauge::StartIncreaseGauge() {
	// 半透明にする
	sprite_["SheathBar"].material.color.A = 120.0f;
	isIncrease_ = true;
	t_ = 0.0f;
}

void SheathGauge::IncreaseGauge() {
	// ゲージがなくなったら増加開始
	if (GetValueEmpty()) {
		if (!isIncrease_) { StartIncreaseGauge(); }
	}

	if (!isIncrease_) { 
		sprite_["SheathBar"].material.color.A = 255.0f;
		return; 
	}

	t_++;
	t_ = std::clamp<float>(t_, 0.0f, increaseTime * 60.0f);

	// ゲージを増加
	value_ = LWP::Utility::Interpolation::LerpF(0.0f, maxValue_, t_ / (increaseTime * 60.0f));

	// 増加時間終了
	if (t_ >= increaseTime * 60.0f) { isIncrease_ = false; }
}