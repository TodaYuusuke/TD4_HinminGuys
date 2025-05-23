#pragma once
#include "../ISheathSystemState.h"

/// <summary>
/// 鞘の回収可能状態
/// </summary>
class Collect : public ISheathSystemState {
public:
	// コンストラクタ
	Collect(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders);
	// デストラクタ
	~Collect() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 回収コマンド
	/// </summary>
	void Command() override;

	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand() override;

private:
	/// <summary>
	/// 鞘を回収するときの挙動
	/// </summary>
	void CollectMove();

private:
	float SmoothDampF(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
	LWP::Math::Vector3 SmoothDamp(LWP::Math::Vector3 current, LWP::Math::Vector3 target, LWP::Math::Vector3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

private:
	LWP::Math::Vector3 velocity_;
	LWP::Math::Vector3 start_;
	LWP::Math::Vector3 end_;
	float t_;
};