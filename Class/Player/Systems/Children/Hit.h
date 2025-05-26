#pragma once
#include "../ISystem.h"
#include "../../Gauge/HP/HP.h"
#include "../EventOrder.h"

class Hit : public ISystem {
public:
	enum class EventOrderState {
		kInvinsible
	};

public:
	Hit(LWP::Object::Camera* camera, Player* player);
	~Hit() override = default;

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
	/// 
	/// </summary>
	void DebugGUI() override;

	/// <summary>
	/// jsonファイルの作成
	/// </summary>
	void CreateJsonFIle() override;

private:
	/// <summary>
	/// 被弾時の更新処理
	/// </summary>
	void HitUpdate();

	/// <summary>
	/// アクションイベントの作成
	/// </summary>
	void CreateEventOrders();

public:
#pragma region Setter
	void StartDamage(const float& damageValue, const float& multiply = 1.0f) {
		//hp_.SetDeltaValue(damageValue);
		//hp_.SetMultiply(multiply);
		//hp_.Hit();
		eventOrders_[(int)EventOrderState::kInvinsible].Start();
	}
	/// <summary>
	/// 被弾演出開始
	/// </summary>
	void StartEffect() { isHit_ = true; }
	/// <summary>
	/// HPの最大値を設定
	/// </summary>
	/// <param name="maxHP"></param>
	void SetMaxHP(const float& maxHP) {
		//hp_.SetMaxValue(maxHP);
	}
	/// <summary>
	/// 演出終了時間を設定
	/// </summary>
	/// <param name="endFrame">演出終了時間</param>
	void SetEndFrame(float endFrame) {
		endFrame_ = endFrame;
		currentFrame_ = endFrame_;
	}
#pragma endregion

#pragma region Getter
	/// <summary>
	/// 無敵時間中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsInvinsible() { 
		if (eventOrders_[(int)EventOrderState::kInvinsible].GetCurrentTimeEvent().name == "InvinsibleTime") {
			return true;
		}
		return false;
	}
	/// <summary>
	/// 被弾演出中か
	/// </summary>
	/// <returns></returns>
	const bool& GetIsHitEffect() { return isHit_; }
#pragma endregion

private:// jsonで保存する値
	// 無敵時間
	float invinsibleTime = 1.0f;

private:
	std::map<int, EventOrder> eventOrders_;

	// 演出終了時間
	float endFrame_;
	// 経過時間
	float currentFrame_;

	// 被弾演出中か
	bool isHit_ = false;
	bool isPreHit_ = false;
};