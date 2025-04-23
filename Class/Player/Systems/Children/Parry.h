#pragma once
#include "../ISystem.h"

struct TimeEvent {
	float graceTime; // 猶予時間
	std::string name;// 今の状態の名前
};

/// <summary>
/// 自機のパリィ機能をまとめたクラス
/// </summary>
class Parry : public ISystem {
public:
	// コンストラクタ
	Parry(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Parry() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 全ての数値をリセット
	/// </summary>
	void Reset() override;

	/// <summary>
	/// ImGuiによるパラメータ表示
	/// </summary>
	void DebugGui();

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// パリィの情報を作成
	/// </summary>
	std::vector<TimeEvent> CreateParryData();

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// パリィ中に起こる状態をまとめたクラスを取得
	/// </summary>
	/// <returns></returns>
	std::vector<TimeEvent> GetTimeEvents() { return timeEvents_; }
#pragma endregion

private:// 定数
	// パリィ発動までにかかる時間
	const float kSwingTime = 60.0f * 0.0f;
	// 通常パリィの猶予時間[frame * 秒]
	const float kGoodParryTime = 60.0f * 0.6f;
	// ジャストパリィの猶予時間[frame * 秒]
	const float kJustParryTime = 60.0f * 0.1f;
	// パリィの硬直[frame * 秒]
	const float kRecoveryTime = 60.0f * 0.0f;

private:
	// frameごとに起きる状態をまとめたリスト
	std::vector<TimeEvent> timeEvents_;

	// デバッグ用のイベントリスト(ImGuiにしか使ってないから気にしないで)
	std::vector<TimeEvent> debugTimeEvents_;

	// 経過時間
	float currentTime_;
};