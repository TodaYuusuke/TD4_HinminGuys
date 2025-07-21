#pragma once
#include "../Adapter/Adapter.h"
#include "../Player/Gauge/HP/HP.h"
#include "../Player/Gauge/Sheath/SheathGauge.h"
#include "AfterimageMotion.h"

class Player;
class UIManager {
public:
	struct GuideUI {
		LWP::Primitive::NormalSprite sprite;
		LWP::Primitive::ClipSprite coolTimeFilter;		// クールタイムのフィルタ
		LWP::Math::Vector2 coolTimeFilterSplitSize;
		bool isActive = false;// 可能状態か
		bool isPreActive = false;// 可能状態か
	};

public:
	// コンストラクタ
	UIManager(Player* player);
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

	/// <summary>
	/// クールタイム演出
	/// </summary>
	void CoolTimeFilterUpdate();

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
	/// <summary>
	/// 鞘ゲージが0になっているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsSheathEmpty() { return sheathGauge_.GetValueEmpty(); }
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

private:// 外部から受け取る変数
	Player* player_;

private:
	LWP::Utility::JsonIO json_;

	// HPゲージ
	HP hp_;
	// 鞘ゲージ
	SheathGauge sheathGauge_;

	// 操作ガイド
	std::vector<GuideUI> guideUI_;
	// 残像
	std::vector<AfterimageMotion*> afterimageUI_;
};