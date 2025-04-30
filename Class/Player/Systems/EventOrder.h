#pragma once
#include "../Adapter/Adapter.h"

// アクションイベント
struct TimeEvent {
	float graceTime; // 猶予時間
	std::string name;// 今の状態の名前
};

/// <summary>
/// フレーム単位で発生するアクションイベントを管理するクラス
/// </summary>
class EventOrder {
public:
	// コンストラクタ
	EventOrder() = default;
	// デストラクタ
	~EventOrder() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 開始
	/// </summary>
	void Start();

	/// <summary>
	/// 全ての数値をリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// アクションイベントを作成
	/// </summary>
	/// <param name="timeEvent">アクションイベント</param>
	void CreateTimeEvent(TimeEvent timeEvent);

	/// <summary>
	/// ImGuiによるパラメータ表示
	/// </summary>
	void DebugGUI();

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// アクションイベントをまとめたリストを取得
	/// </summary>
	/// <returns></returns>
	std::vector<TimeEvent> GetTimeEvents() { return timeEvents_; }
	/// <summary>
	/// 現在実行中のアクションイベントを取得
	/// </summary>
	/// <returns>アクションイベントがあるときのみ値を返す</returns>
	TimeEvent GetCurrentTimeEvent() {
		if (!timeEvents_.empty()) { return timeEvents_[0]; }
		return TimeEvent();
	}
	/// <summary>
	/// 機能が使える状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActive_; }
	/// <summary>
	/// 全てのアクションイベントが終了しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEnd() { return timeEvents_.empty(); }
#pragma endregion

private:
	// フレーム単位で発生するアクションイベントをまとめたリスト(0番が今実行しているアクションイベント)
	std::vector<TimeEvent> timeEvents_;
	// 実行予定のアクションイベント
	std::vector<TimeEvent> timeEventSchedules_;

	// 経過時間
	float currentTime_;

	// 機能が使える状態か
	bool isActive_;
};

