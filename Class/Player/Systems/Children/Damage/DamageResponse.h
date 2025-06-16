#pragma once
#include "../../ISystem.h"
#include "../../../Gauge/HP/HP.h"

struct DamageResponseJsonData {
	// 無敵時間
	float invinsibleTime = 2.2f;

	// スタン時間
	float stunTime = 1.8f;
	// スタンキャンセル時間
	float stunCancelTime = 1.0f;
};

/// <summary>
/// ダメージリアクション
/// </summary>
class DamageResponse : public ISystem {
public:
	enum class EventOrderState {
		kInvinsible,
		kStun
	};

public:
	DamageResponse(LWP::Object::Camera* camera, Player* player);
	~DamageResponse() override = default;

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

	/// <summary>
	/// 無敵開始
	/// </summary>
	void StartInvinsible();
	/// <summary>
	/// 被弾演出開始
	/// </summary>
	void StartEffect() { isHit_ = true; }

private:
	/// <summary>
	/// 被弾時の更新処理
	/// </summary>
	void HitUpdate();

	/// <summary>
	/// アクションイベントの作成
	/// </summary>
	void CreateEventOrders();
	/// <summary>
	/// アクションイベントの作成
	/// </summary>
	void CreateInvinsibleEventOrder();
	/// <summary>
	/// アクションイベントの作成
	/// </summary>
	void CreateStunEventOrder();

	/// <summary>
	/// スタンのアクションイベント
	/// </summary>
	void CheckStunEventOrder();
	/// <summary>
	/// 無敵のアクションイベント
	/// </summary>
	void CheckInvinsibleEventOrder();

public:
#pragma region Getter
	/// <summary>
	/// jsonに保存する値を取得
	/// </summary>
	DamageResponseJsonData GetJsonData() { return jsonData_; }
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
	/// スタン時間中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsStun() {
		if (eventOrders_[(int)EventOrderState::kStun].GetCurrentTimeEvent().name == "StunTime") {
			return true;
		}
		return false;
	}
	/// <summary>
	/// スタンキャンセル時間中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsStunCancel() {
		if (eventOrders_[(int)EventOrderState::kStun].GetCurrentTimeEvent().name == "CancelTime") {
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

#pragma region Setter
	/// <summary>
	/// jsonに保存する値を設定
	/// </summary>
	/// <param name="jsonData"></param>
	void SetJsonData(const DamageResponseJsonData& jsonData) { jsonData_ = jsonData; }
	/// <summary>
	/// HPの最大値を設定
	/// </summary>
	/// <param name="maxHP"></param>
	void SetMaxHP(const float& maxHP) {
		maxHP;
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

private:// jsonで保存する値
	DamageResponseJsonData jsonData_;

private:
	std::map<int, EventOrder> eventOrders_;

	// 前のアクションイベント
	std::string preEventOrder_;

	// 演出終了時間
	float endFrame_;
	// 経過時間
	float currentFrame_;

	// 被弾演出中か
	bool isHit_ = false;
	bool isPreHit_ = false;
};