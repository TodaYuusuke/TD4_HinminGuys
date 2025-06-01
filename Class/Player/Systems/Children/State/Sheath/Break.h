#pragma once
#include "../ISheathSystemState.h"
#include "../../../EventOrder.h"

/// <summary>
/// 鞘を投擲可能状態
/// </summary>
class Break : public ISheathSystemState {
public:
	// コンストラクタ
	Break(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders);
	// デストラクタ
	~Break() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	///	鞘破壊されている時のコマンド
	/// </summary>
	void Command() override;

	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand() override;

	/// <summary>
	/// 値をリセット
	/// </summary>
	void Reset();

private:
	/// <summary>
	/// アクションイベント応じて処理を変える
	/// </summary>
	void CheckBreakState();

private:
	LWP::Math::Vector3 velocity_;
	LWP::Math::Vector3 start_;
	LWP::Math::Vector3 end_;
	float t_;
};