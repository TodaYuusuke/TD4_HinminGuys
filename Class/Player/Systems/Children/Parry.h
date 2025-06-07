#pragma once
#include "../ISystem.h"

struct ParryJsonData {
	// パリィ発動までにかかる時間[秒]
	float kSwingTime = 0.0f;
	// 通常パリィの猶予時間[秒]
	float kGoodParryTime = 0.6f;
	// ジャストパリィの猶予時間[秒]
	float kJustParryTime = 0.2f;
	// パリィの硬直[秒]
	float kRecoveryTime = 0.0f;

	// ジャストパリィ成功時の無敵時間
	float successJustParryInvinsible = 2.0f;
	// 弱パリィ成功時の無敵時間
	float successGoodParryInvinsible = 1.0f;

	// ジャストパリィ時の鞘ゲージの減少量
	float justParryDecrement = 50.0f;
	// 甘パリィ時の鞘ゲージの減少量
	float goodParryDecrement = 10.0f;

	// ジャストパリィ時のノックバック量
	float justParryKnockBackMovement = 6.0f;
	// ジャストパリィ時のノックバック終了時間
	float justParryKnockBackFinishTime = 60.0f;

	// クールタイム
	float coolTime = 1.0f;
};

/// <summary>
/// 自機のパリィ機能をまとめたクラス
/// </summary>
class Parry : public ISystem {
public:
	enum class ParryInvinsibleState{
		kRunning = 0,	// パリィ中
		kGood = 1,		// 弱パリィ成功
		kJust = 2		// ジャストパリィ成功
	};

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
	void DebugGUI() override;

	/// <summary>
	/// jsonファイルの作成
	/// </summary>
	void CreateJsonFIle() override;

	/// <summary>
	/// パリィのコマンド
	/// </summary>
	void Command();

	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand();

private:
	/// <summary>
	/// 当たり判定を作成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// アクションイベントを生成
	/// </summary>
	void CreateEventOrder();

	/// <summary>
	/// パリィ中の無敵イベントを生成
	/// </summary>
	void CreateParryInvinsibleEventOrder();
	/// <summary>
	/// ジャストパリィ成功時の無敵イベントを生成
	/// </summary>
	void CreateJustParryInvinsibleEventOrder();
	/// <summary>
	/// 弱パリィ成功時の無敵イベントを生成
	/// </summary>
	void CreateGoodParryInvinsibleEventOrder();

	/// <summary>
	/// パリィの状態を確認
	/// </summary>
	void CheckParryState();

	/// <summary>
	/// ノックバックの計算
	/// </summary>
	void KnockBackUpdate();

	/// <summary>
	/// クールタイムの更新
	/// </summary>
	void CoolTimeUpdate();

public:// Getter, Setter
	/// <summary>
	/// クールタイムが終了しているかを確認
	/// </summary>
	/// <returns>true = 既定の時間を越している</returns>
	bool CheckCoolTime() {
		// 既定の時間を越している
		if (currentCoolTime_ <= 0.0f) {
			return true;
		}
		return false;
	}

#pragma region Getter
	/// <summary>
	/// jsonに保存する値を取得
	/// </summary>
	ParryJsonData GetJsonData() { return jsonData_; }
	/// <summary>
	/// パリィされた相手の座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetParryTargetPos() { return parryTargetPos_; }
	/// <summary>
	/// 無敵時間中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsInvinsible() {
		for (int i = 0; i < eventOrders_.size(); i++) {
			if (eventOrders_[i].GetCurrentTimeEvent().name == "InvinsibleTime") {
				return true;
			}
		}
		return false;
	}
	/// <summary>
	/// ジャストパリィが成功しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetSuccessJustParry() { return isJustParry_; }
	/// <summary>
	/// 弱パリィが成功しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetSuccessGoodParry() { return isGoodParry_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// jsonに保存する値を設定
	/// </summary>
	/// <param name="jsonData"></param>
	void SetJsonData(const ParryJsonData& jsonData) { jsonData_ = jsonData; }
	/// <summary>
	/// クールタイムを設定
	/// </summary>
	/// <param name="time"></param>
	void SetCoolTime() { currentCoolTime_ = jsonData_.coolTime * 60.0f; }
#pragma endregion

private:// jsonで保存する値
	ParryJsonData jsonData_;

private:
	// アクションイベント集(無敵に関するものだけ)
	std::map<int, EventOrder> eventOrders_;

	// パリィ判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;

	// パリィできた攻撃をしてきた相手の座標
	LWP::Math::Vector3 parryTargetPos_;

	// ジャストパリィ時のノックバック
	LWP::Math::Vector3 justParryKnockBack_;

	LWP::Math::Vector3 start_;
	LWP::Math::Vector3 end_;

	float t_;

	// クールタイムの経過時間
	float currentCoolTime_;

	bool isJustParry_;
	bool isGoodParry_;
};