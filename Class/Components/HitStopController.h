#pragma once
#include "Adapter.h"

class HitStopController {
public:
	// コンストラクタ
	HitStopController();
	// デストラクタ
	~HitStopController() = default;

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static HitStopController* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public:// Getter, Setter
	/// <summary>
	/// ヒットストップ開始
	/// </summary>
	/// <param name="time"></param>
	/// <param name="multiply"></param>
	void Start(const float& time, const float& multiply = 0.2f) {
		SetHitStopTime(time);
		SetTimeMultiply(multiply);
	}
#pragma region Getter
	/// <summary>
	/// ヒットストップ時間を取得
	/// </summary>
	float GetHitStopTime() { return hitStopTime_; }
	/// <summary>
	/// ヒットストップ時間を取得
	/// </summary>
	float GetCurrentFrame() { return currentFrame_; }
	/// <summary>
	/// 経過時間を取得(既存のΔタイムだと数値のばらつきがありジッターが起きてしまうので固定値にする)
	/// </summary>
	/// <returns></returns>
	float GetDeltaTime() { return 1.0f * multiply_; }
	/// <summary>
	/// ヒットストップが終了しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFinish() { return isFinish_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// ヒットストップ時間を設定
	/// </summary>
	/// <param name="time"></param>
	void SetHitStopTime(const float& time) {
		hitStopTime_ = time * fps;
		currentFrame_ = hitStopTime_;
	}
	/// <summary>
	/// ヒットストップのデルタタイムの係数を設定
	/// </summary>
	/// <param name="multiply"></param>
	void SetTimeMultiply(const float& multiply) { 
		LWP::Information::SetDeltaTimeMultiply(multiply); 
		multiply_ = multiply;
	}
#pragma endregion

private:
	// フレームパーセカンド
	float fps = 60.0f;

	// ヒットストップする時間
	float hitStopTime_;
	// 経過時間
	float currentFrame_;

	float multiply_ = 1.0f;

	// 終了しているかを取得
	bool isFinish_;
};