#pragma once
#include "../ISheathSystemState.h"
#include "../../../EventOrder.h"

/// <summary>
/// 鞘がない状態
/// </summary>
class SwordDrawn : public ISheathSystemState {
public:
	// コンストラクタ
	SwordDrawn(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders);
	// デストラクタ
	~SwordDrawn() override;

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

};