#pragma once
#include "../Adapter/Adapter.h"

enum class CoolTimeTarget {
	kParry,		// パリィ
	kEvasion,	// 回避
	kSheath,	// 鞘
	kCount		// 個数カウント
};

class CoolTimer {
public:
	struct CoolTimeData {
		float coolTime;			// クールタイム
		float maxCoolTime;		// クールタイムの最大時間
		bool isFinish = true;	// 終了しているか
	};

public:
	// コンストラクタ
	CoolTimer();
	// デストラクタ
	~CoolTimer() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// パリィのクールタイム情報を取得
	/// </summary>
	CoolTimeData GetParryCoolTimeData() { return coolTimes_[CoolTimeTarget::kParry]; }
	/// <summary>
	/// 回避のクールタイム情報を取得
	/// </summary>
	CoolTimeData GetEvasionCoolTimeData() { return coolTimes_[CoolTimeTarget::kEvasion]; }
	/// <summary>
	/// 鞘のクールタイム情報を取得
	/// </summary>
	CoolTimeData GetSheathCoolTimeData() { return coolTimes_[CoolTimeTarget::kSheath]; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// パリィのクールタイムを設定
	/// </summary>
	/// <param name="coolTime">クールタイム[秒]</param>
	void SetParryCoolTime(const float& coolTime){ 
		coolTimes_[CoolTimeTarget::kParry].coolTime = coolTime * 60.0f; 
		coolTimes_[CoolTimeTarget::kParry].maxCoolTime = coolTime * 60.0f;
	}
	/// <summary>
	/// 回避のクールタイムを設定
	/// </summary>
	/// <param name="coolTime">クールタイム[秒]</param>
	void SetEvasionCoolTime(const float& coolTime){ 
		coolTimes_[CoolTimeTarget::kEvasion].coolTime = coolTime * 60.0f;
		coolTimes_[CoolTimeTarget::kEvasion].maxCoolTime = coolTime * 60.0f;
	}
	/// <summary>
	/// 鞘のクールタイムを設定
	/// </summary>
	/// <param name="coolTime">クールタイム[秒]</param>
	void SetSheathCoolTime(const float& coolTime){ 
		coolTimes_[CoolTimeTarget::kSheath].coolTime = coolTime * 60.0f; 
		coolTimes_[CoolTimeTarget::kSheath].maxCoolTime = coolTime * 60.0f;
	}
#pragma endregion

private:
	std::map<CoolTimeTarget, CoolTimeData> coolTimes_;
};