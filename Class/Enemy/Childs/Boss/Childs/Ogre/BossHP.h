#pragma once
#include "../../../../../Player/Gauge/IGauge.h"

class BossHP : public IGauge {
public:
	// コンストラクタ
	BossHP();
	// デストラクタ
	~BossHP() override = default;

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
		// ゲージ量 += 変化量 * 倍率
		value_ -= deltaValue_ * multiply_;
	}

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// HPが無くなっているかを取得
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

private:

};