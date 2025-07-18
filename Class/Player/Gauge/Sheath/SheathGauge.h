#pragma once
#include "../IGauge.h"
#include "../../../Components/EventOrder.h"

class SheathGauge : public IGauge {
public:
	// コンストラクタ
	SheathGauge();
	// デストラクタ
	~SheathGauge() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGuiによるパラメータ表示
	/// </summary>
	void DebugGUI() override;

	/// <summary>
	/// ゲージ量を変える
	/// </summary>
	void Hit() override {
		// ゲージ増加中なら変更させない
		if (isIncrease_) { return; }
		// ゲージ量 -= 変化量 * 倍率
		value_ -= deltaValue_ * multiply_;
	}

private:
	/// <summary>
	/// ゲージの増加開始
	/// </summary>
	void StartIncreaseGauge();

	/// <summary>
	/// ゲージの増加
	/// </summary>
	void IncreaseGauge();

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// ゲージが増加中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsIncrease() { return isIncrease_; }
	/// <summary>
	/// ゲージが無くなっているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetValueEmpty() {
		if (value_ <= 0.0f) { return true; }
		return false;
	}
#pragma endregion

#pragma region Setter

#pragma endregion

private:// jsonで保存する値
	// ゲージの回復にかかる時間[秒]
	float increaseTime = 6.0f;

private:
	// 体力増加のイージング
	float t_;

	// ゲージの上昇を開始するか
	bool isIncrease_ = false;
};