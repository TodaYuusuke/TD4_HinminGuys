#pragma once
#include "../Adapter/Adapter.h"
#include "../Player/Gauge/HP/HP.h"
#include "../Player/Gauge/Sheath/SheathGauge.h"

class UIManager {
public:
	// コンストラクタ
	UIManager();
	// デストラクタ
	~UIManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグ用のタブを表示(Debug時のみ)
	/// </summary>
	void DebugGUI();

public:
#pragma region Getter
	/// <summary>
	/// HPゲージを取得
	/// </summary>
	/// <returns></returns>
	HP GetHPGauge() { return hp_; }
	/// <summary>
	/// 鞘ゲージを取得
	/// </summary>
	/// <returns></returns>
	SheathGauge GetSheathGauge() { return sheathGauge_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// HPゲージの割合変更
	/// </summary>
	/// <param name="damageValue"></param>
	/// <param name="multiply"></param>
	void ChangeHPGauge(const float& damageValue, const float& multiply = 1.0f) {
		hp_.SetDeltaValue(damageValue);
		hp_.SetMultiply(multiply);
		hp_.Hit();
	}
	/// <summary>
	/// 鞘ゲージの割合変更
	/// </summary>
	/// <param name="damageValue"></param>
	/// <param name="multiply"></param>
	void ChangeSheathGauge(const float& damageValue, const float& multiply = 1.0f) {
		sheathGauge_.SetDeltaValue(damageValue);
		sheathGauge_.SetMultiply(multiply);
		sheathGauge_.Hit();
	}
	/// <summary>
	/// HPの最大値を設定
	/// </summary>
	/// <param name="maxHP"></param>
	void SetMaxHP(const float& maxHP) {
		hp_.SetMaxValue(maxHP);
	}
#pragma endregion

private:
	// HPゲージ
	HP hp_;
	// 鞘ゲージ
	SheathGauge sheathGauge_;
};