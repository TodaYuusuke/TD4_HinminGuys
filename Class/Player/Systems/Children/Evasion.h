#pragma once
#include "../ISystem.h"

class Evasion : public ISystem {
private:
	struct EaseData {
		LWP::Math::Vector3* target;
		LWP::Math::Vector3 start;
		LWP::Math::Vector3 end;
		float endSecond;
		float t;
		bool isEnd;
	};

	enum class EventOrderState {
		kInvincible		 = 0,		// 無敵
		kAcceleration	 = 1		// 加速
	};

public:
	// コンストラクタ
	Evasion(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Evasion() override = default;

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
	void DebugGUI() override;

	/// <summary>
	/// 回避のコマンド
	/// </summary>
	void Command();

	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand();

private:
	/// <summary>
	/// 無敵タイミングのアクションイベントを作成
	/// </summary>
	void CreateInvincibleEventOrder();
	/// <summary>
	/// 加速タイミングのアクションイベントを作成
	/// </summary>
	void CreateAccelerationEventOrder();

	/// <summary>
	/// アクションイベントの生成
	/// </summary>
	void CreateEventOrder();

	/// <summary>
	/// 回避の状態を確認
	/// </summary>
	void CheckEvasionState();

	/// <summary>
	/// ダッシュに移行する条件を満たしているかを確認
	/// </summary>
	void CheckDash();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 回避速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 向いている方向を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetRadian() { return radian_; }
	/// <summary>
	/// 無敵状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsInvinsible() {
		if (eventOrders_[(int)EventOrderState::kInvincible].GetCurrentTimeEvent().name == "InvinsibleTime") {
			return true;
		}
		return false;
	}
	/// <summary>
	/// ダッシュするのに必要なボタンを長押ししているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDash() {
		if (pressTime_ >= dashButtonHoldSeconds * 60.0f) { return true; }
		return false;
	}
#pragma endregion

#pragma region Setter
	/// <summary>
	/// ダッシュ状態にするかを設定
	/// </summary>
	/// <param name="isDash"></param>
	void SetIsDash(const bool& isDash) {
		if (isDash) {
			pressTime_ = dashButtonHoldSeconds * 60.0f; 
			return;
		}
		pressTime_ = 0.0f;
	}
#pragma endregion

private:// jsonで保存する値
	// 回避の終了時間
	float evasionFinishTime = 0.3f;

	// 回避の無敵発動までにかかる時間[秒]
	float invinsibleSwingTime = 0.0f;
	// 回避の無敵時間[秒]
	float invinsibleTime = 0.3f;
	// 回避の無敵硬直[秒]
	float invinsibleRecoveryTime = 0.0f;

	// 回避の加速発動までにかかる時間[秒]
	float accelerationSwingTime = 0.0f;
	// 回避の加速時間[秒]
	float accelerationTime = 0.3f;
	// 回避の加速硬直[秒]
	float accelerationRecoveryTime = 0.0f;

	// ダッシュ移行するのに必要なボタンを押す時間
	float dashButtonHoldSeconds = 60.0f * 0.3f;

	// 回避速度の係数
	float moveMultiply = 1.0f;

	// 回避の移動量
	LWP::Math::Vector3 evasionMovement = { 0.0f, 0.0f, 1.0f };

private:// プライベートな変数
	// 回避時の速度
	LWP::Math::Vector3 velocity_;
	// 回避時の角度(ラジアン)
	LWP::Math::Vector3 radian_;

	// アニメーションの再生速度をイージング
	LWP::Resource::Motion animationPlaySpeed_;
	// アニメーションの再生速度
	LWP::Math::Vector3 animPlaySpeed_;

	// 回避速度のイージング
	EaseData easeData_;

	// アクションイベント集
	std::map<int, EventOrder> eventOrders_;

	// 回避ボタンを押した時間
	float pressTime_;
};