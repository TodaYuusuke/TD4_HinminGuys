#pragma once
#include "Adapter.h"

class IGauge {
public:
	// コンストラクタ
	IGauge() = default;
	// デストラクタ
	virtual ~IGauge() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// ImGuiによるパラメータ表示
	/// </summary>
	virtual void DebugGUI() = 0;

	/// <summary>
	/// ゲージ量を変える
	/// </summary>
	void Hit() {
		// ゲージ量 += 変化量 * 倍率
		value_ += deltaValue_ * multiply_;
	}

	/// <summary>
	/// ゲージ量を最大値に戻す
	/// </summary>
	void ResetValue() { value_ = maxValue_; }

	/// <summary>
	/// ゲージのサイズを計算
	/// </summary>
	void ColGaugeSize(std::string name) {
		// HPバーの長さ計算
		// 今のHPバーのスケール = 最大HPの時のバーのサイズ × (今のHP ÷ 最大HP)
		sprite_[name].worldTF.scale.x = (maxSize_.x * (value_ / maxValue_));

		isPreHit_ = isHit_;
	}

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// ゲージが減る瞬間を取得(使用する場合は更新処理でisPreHit = isHit)
	/// </summary>
	/// <returns></returns>
	inline bool GetHitTrigger() {
		if (isHit_ && !isPreHit_) { return true; }
		return false;
	}
	/// <summary>
	/// 残り体力の割合[%]を取得
	/// </summary>
	/// <returns></returns>
	inline float GetPercent() { return value_ / maxValue_ * 100.0f; }
	/// <summary>
	/// 指定したパーセント > 体力の割合が下かを取得
	/// </summary>
	/// <param name="percent"></param>
	/// <returns></returns>
	inline bool GetIsBelowPercent(float percent) {
		if (percent > GetPercent()) { return true; }
		return false;
	}
#pragma endregion

#pragma region Setter
	/// <summary>
	/// ゲージの変動量を設定
	/// </summary>
	/// <param name="deltaValue">ゲージの変動量</param>
	void SetDeltaValue(const float& deltaValue) { this->deltaValue_ = deltaValue; }
	/// <summary>
	/// ゲージの変動倍率を設定
	/// </summary>
	/// <param name="multiply">ゲージの変動倍率</param>
	void SetMultiply(const float& multiply) { this->multiply_ = multiply; }
	/// <summary>
	/// ゲージの最大値を設定
	/// </summary>
	/// <param name="maxValue">ゲージの最大値</param>
	void SetMaxValue(const float& maxValue) { this->maxValue_ = maxValue; }
#pragma endregion

protected:
	// jsonに値を保存
	LWP::Utility::JsonIO json_;

	// ゲージの描画
	std::map<std::string, LWP::Primitive::Sprite> sprite_;

	// ゲージの大きさ
	LWP::Math::Vector2 size_;
	// 体力画像の最大サイズ(今回でいうと初期の画像サイズ)
	LWP::Math::Vector2 maxSize_;

	// ゲージの変動量						
	float deltaValue_;
	// ゲージの上昇倍率
	float multiply_;

	// 値
	float value_;
	// 最大値
	float maxValue_;

	bool isHit_;
	bool isPreHit_;
};