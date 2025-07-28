#include "DamageEffectEmitter.h"

LWP::Math::Vector3 DamageEffect::color = { 0.0f,127.0f,255.0f };
float DamageEffect::divide = 20.0f;
float DamageEffect::maxDisplayTime = 1.5f;
float DamageEffect::vanishThreshold = 0.5f;
float DamageEffect::maxScale = 1.0f;
float DamageEffect::widthSpacing = 24.0f;

void DamageEffectEmitter::Initialize() {

	effects_.clear();
	currentZValue_ = 10.0f;
	// 初期化
	json_.Init("DamageEffectEmitter.json");

	json_.BeginGroup("DamageEffectEmitter")
		.AddValue<LWP::Math::Vector3>("Color", &DamageEffect::color)
		.AddValue<float>("Divide", &DamageEffect::divide)
		.AddValue<float>("MaxDisplayTime", &DamageEffect::maxDisplayTime)
		.AddValue<float>("VanishThreshold", &DamageEffect::vanishThreshold)
		.AddValue<float>("MaxScale", &DamageEffect::maxScale)
		.AddValue<float>("WidthSpacing", &DamageEffect::widthSpacing)
		.EndGroup()
		.CheckJsonFile();

}

void DamageEffectEmitter::Update() {

	//終了したエフェクトを削除
	effects_.remove_if([](const DamageEffect& effect) {

		//表示時間超過で消す
		if (effect.remainingDisplayTime <= 0) {
			return true;
		}

		return false;

		});

	//エフェクトがある時のみ更新
	if (not effects_.empty()) {

		//ワールド座標からスクリーン座標に変換
		LWP::Math::Matrix4x4 viewProjectionViewport =
			camera_->GetViewProjection() *
			LWP::Math::Matrix4x4::CreateViewportMatrix(0, 0, LWP::Info::GetWindowWidthF(), LWP::Info::GetWindowHeightF(), 0.0f, 1.0f);

		//各エフェクト更新
		for (DamageEffect& effect : effects_) {

			//画像の更新
			for (int32_t i = 0; LWP::Primitive::SequenceSprite& sprite : effect.sprites) {

				sprite.worldTF.translation = (effect.position) * viewProjectionViewport;
				sprite.worldTF.translation.z = effect.zValue;
				sprite.worldTF.translation.x += DamageEffect::widthSpacing * float(i);

				//消える時の演出
				if (effect.isVanish) {

					/*sprite.material.color.A = char(LWP::Utility::Interpolation::LerpF(255.0f, 0.0f,
						LWP::Utility::Easing::Liner(std::clamp((DamageEffect::vanishThreshold -
							(effect.remainingDisplayTime + (DamageEffect::maxDisplayTime / DamageEffect::divide) * i)) /
							(DamageEffect::maxDisplayTime / DamageEffect::divide), 0.0f, 1.0f))));*/
					//時間を分割して1つずつ消すようにする
					sprite.worldTF.scale = LWP::Utility::Interpolation::Lerp({ DamageEffect::maxScale * 0.5f,DamageEffect::maxScale * 0.5f,1.0f }, { 0.0f, 0.0f, 1.0f },
						LWP::Utility::Easing::OutQuart(std::clamp((DamageEffect::vanishThreshold -
							(effect.remainingDisplayTime + (DamageEffect::maxDisplayTime / DamageEffect::divide) * i)) /
							(DamageEffect::maxDisplayTime / DamageEffect::divide), 0.0f, 1.0f)));
				}
				//出現時のイージング
				else {
					/*sprite.material.color.A = char(LWP::Utility::Interpolation::LerpF(0.0f, 255.0f,
						LWP::Utility::Easing::Liner(std::clamp((DamageEffect::maxDisplayTime -
							(effect.remainingDisplayTime + (DamageEffect::maxDisplayTime / DamageEffect::divide) * i)) /
							(DamageEffect::maxDisplayTime / DamageEffect::divide), 0.0f, 1.0f))));*/
					//時間を分割して1つずつ表示するようにする
					sprite.worldTF.scale = LWP::Utility::Interpolation::Lerp({ DamageEffect::maxScale,DamageEffect::maxScale,1.0f }, { DamageEffect::maxScale * 0.5f, DamageEffect::maxScale * 0.5f, 1.0f },
						LWP::Utility::Easing::OutBounce(std::clamp((DamageEffect::maxDisplayTime -
							(effect.remainingDisplayTime + (DamageEffect::maxDisplayTime / DamageEffect::divide) * i)) /
							(DamageEffect::maxDisplayTime / DamageEffect::divide), 0.0f, 1.0f)));
				}

				//残り時間半分から消していく
				if (effect.remainingDisplayTime <= DamageEffect::vanishThreshold) {
					effect.isVanish = true;
				}

				i++;

			}

			effect.remainingDisplayTime -= 1.0f * LWP::Info::GetDeltaTimeF();

		}

	}

}

void DamageEffectEmitter::DebugGUI() {

	if (ImGui::TreeNode("Damage Effect")) {
		//パラメータ編集
		json_.DebugGUI();

		ImGui::TreePop();

	}

}

void DamageEffectEmitter::AddEffect(const float& damage, const LWP::Math::Vector3& position)
{

	effects_.push_back(DamageEffect());

	//桁数を保持
	effects_.back().digit = int(std::log10(int(damage))) + 1;
	//ポジションを保持
	effects_.back().position = position;
	effects_.back().zValue = currentZValue_;
	effects_.back().damage = damage;
	effects_.back().remainingDisplayTime = DamageEffect::maxDisplayTime;

	//ワールド座標からスクリーン座標に変換
	LWP::Math::Matrix4x4 viewProjectionViewport =
		camera_->GetViewProjection() *
		LWP::Math::Matrix4x4::CreateViewportMatrix(0, 0, LWP::Info::GetWindowWidthF(), LWP::Info::GetWindowHeightF(), 0.0f, 1.0f);

	//割られる数
	int dividedNum = int(effects_.back().damage);

	//桁数分リソースを追加
	for (int32_t i = 0; i < effects_.back().digit; i++) {

		//分割数
		float shiftVal = 0.1f;

		//割る数
		int divisionNum = (int)std::pow(10, effects_.back().digit - i - 1);

		//割られる数を割る数で割り、数字を決める
		int assignNum = dividedNum / divisionNum;

		effects_.back().sprites.emplace_back();

		LWP::Primitive::SequenceSprite& sprite = effects_.back().sprites.back();

		sprite.LoadTexture("UI/Number.png");
		sprite.SetSplitSize({ 128.0f,128.0f });
		sprite.index = assignNum;
		sprite.worldTF.translation = effects_.back().position * viewProjectionViewport;
		sprite.anchorPoint = { 0.5f,0.5f };
		sprite.material.color.R = char(DamageEffect::color.x);
		sprite.material.color.G = char(DamageEffect::color.y);
		sprite.material.color.B = char(DamageEffect::color.z);
		sprite.material.enableLighting = false;

		//次の桁に移行
		dividedNum = dividedNum % divisionNum;

	}

	currentZValue_ -= 0.001f;

}
