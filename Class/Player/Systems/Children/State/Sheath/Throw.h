#pragma once
#include "../ISheathSystemState.h"
#include "../../../../../Components/EventOrder.h"

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

	/// <summary>
	/// リセット
	/// </summary>
	void Reset() override;

private:
	/// <summary>
	/// アクションイベント応じて処理を変える
	/// </summary>
	void CheckThrowState();

private:
	// 鞘投げ開始
	float startSheathThrowTime = 0.5f * 60.0f;

private:
	LWP::Math::Vector3 velocity_;
	LWP::Math::Vector3 start_;
	LWP::Math::Vector3 end_;

	// 経過時間
	float currentFrame_;

	float t_;
};