#pragma once
#include "../ISheathSystemState.h"
#include "../../../EventOrder.h"

/// <summary>
/// 鞘を投擲可能状態
/// </summary>
class Throw : public ISheathSystemState {
public:
	// コンストラクタ
	Throw(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders);
	// デストラクタ
	~Throw() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	///	投擲コマンド
	/// </summary>
	void Command() override;

	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand() override;

private:
	/// <summary>
	/// アクションイベント応じて処理を変える
	/// </summary>
	void CheckThrowState();

private:
	// さや投げの移動量
	LWP::Math::Vector3 throwMovement_ = {0,0,10};
};