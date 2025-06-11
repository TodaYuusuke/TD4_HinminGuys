#include "DamageEffectEmitter.h"

float DamageEffect::divide = 6.0f;
float DamageEffect::maxDisplayTime = 2.0f;

void DamageEffectEmitter::Initialize() {

	effects_.clear();

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
			for (int32_t i = 0; LWP::Primitive::Sprite& sprite : effect.sprites) {

				sprite.worldTF.translation = (effect.position) * viewProjectionViewport;
				sprite.worldTF.translation.x += effect.widthSpacing * float(i);
				//最大表示時間をスプライトの数+1で分割し、
				sprite.worldTF.scale = LWP::Utility::Interpolation::Lerp({ 0.0f,0.0f,0.0f }, { effect.maxScale, effect.maxScale, 1.0f },
					LWP::Utility::Easing::OutQuint(std::clamp((DamageEffect::maxDisplayTime - 
						(effect.remainingDisplayTime + (DamageEffect::maxDisplayTime / DamageEffect::divide) * i)) /
						(DamageEffect::maxDisplayTime / DamageEffect::divide), 0.0f, 1.0f)));

				i++;

			}

			effect.remainingDisplayTime -= 1.0f * LWP::Info::GetDeltaTimeF();

		}

	}

}

void DamageEffectEmitter::DebugGUI() {

}

void DamageEffectEmitter::AddEffect(const float& damage, const LWP::Math::Vector3& position)
{

	effects_.push_back(DamageEffect());

	//桁数を保持
	effects_.back().digit = std::log10(int(damage)) + 1;
	//ポジションを保持
	effects_.back().position = position;
	effects_.back().damage = damage;
	//最大スケールセット
	effects_.back().maxScale = 1.0f;
	effects_.back().widthSpacing = 64.0f;
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
		int divisionNum = std::pow(10, effects_.back().digit - i - 1);

		//割られる数を割る数で割り、数字を決める
		int assignNum = dividedNum / divisionNum;

		effects_.back().sprites.push_back(LWP::Primitive::Sprite());
		effects_.back().sprites.back().LoadTexture("UI/testNum.png");
		effects_.back().sprites.back().SetSplitSize({ 128.0f,128.0f });
		effects_.back().sprites.back().material.uvTransform.translation.x = shiftVal * float(assignNum);
		effects_.back().sprites.back().worldTF.translation = effects_.back().position * viewProjectionViewport;
		effects_.back().sprites.back().anchorPoint = { 0.5f,0.5f };
		effects_.back().sprites.back().Init();

		//次の桁に移行
		dividedNum = dividedNum % divisionNum;

	}

}
