#pragma once
#include "../IGauge.h"

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

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// HPが無くなっているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetValueEmpty() {
		if (value <= 0.0f) { return true; }
		return false;
	}
#pragma endregion

#pragma region Setter

#pragma endregion

private:// jsonで保存する値

private:

};