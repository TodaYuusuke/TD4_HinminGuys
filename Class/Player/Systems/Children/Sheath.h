#pragma once
#include "../ISystem.h"
#include "State/ISheathSystemState.h"

class Sheath : public ISystem {
public:
	enum class SheathState {
		kThrow = 0,
		kCollect = 1,
		kBreak = 2,
		kInvinsible = 3,
	};

public:
	// コンストラクタ
	Sheath(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Sheath() override = default;

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
	/// 鞘を投げるor鞘に向かってダッシュコマンド
	/// </summary>
	void Command();
	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand();

	/// <summary>
	/// 当たり判定の作成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// 投げるときのアクションイベントを生成
	/// </summary>
	void CreateThrowEventOrder();
	/// <summary>
	/// 回収するときのアクションイベントを生成
	/// </summary>
	void CreateCollectEventOrder();
	/// <summary>
	/// 鞘破壊されているときのアクションイベントを生成
	/// </summary>
	void CreateBreakEventOrder();
	/// <summary>
	/// 無敵のアクションイベントを生成
	/// </summary>
	void CreateInvinsibleEventOrder();

	/// <summary>
	/// 無敵開始
	/// </summary>
	void StartInvinsible() { eventOrders_[(int)SheathState::kInvinsible].Start(); }

	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="pState">次の状態</param>
	void ChangeState(ISheathSystemState* pState);

	/// <summary>
	/// クールタイムの更新
	/// </summary>
	void CoolTimeUpdate();

	/// <summary>
	/// 移動制限
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	LWP::Math::Vector3 ClampToCircle(LWP::Math::Vector3& position);

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

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 鞘のWorldTFを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat GetSheathWorldTF() { return sheathModel_.worldTF; }
	/// <summary>
	/// 移動速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 向いている方向を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetQuat() { return quat_; }
	/// <summary>
	/// 向いている方向を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetRadian() { return radian_; }
	/// <summary>
	/// 鞘の状態を取得
	/// </summary>
	/// <returns></returns>
	ISheathSystemState* GetSheathState() { return state_; }
	/// <summary>
	/// 無敵状態かを判定
	/// </summary>
	/// <returns></returns>
	bool GetIsInvinsible() {
		if (eventOrders_[(int)SheathState::kInvinsible].GetCurrentTimeEvent().name == "InvinsibleTime") {
			return true;
		}
		return false;
	}
	/// <summary>
	/// 行動制限を行うのかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActionRestrict(std::string stateName) {
		if (stateName == state_->GetStateName()) {
			return state_->GetIsActive();
		}
		return false;
	}
#pragma endregion

#pragma region Setter
	void SetSheathPos(const LWP::Math::Vector3& pos) { sheathModel_.worldTF.translation = pos; }
	/// <summary>
	/// 移動速度を設定
	/// </summary>
	/// <param name="velocity">移動速度</param>
	void SetVelocity(const LWP::Math::Vector3& velocity) { velocity_ = velocity; }
	/// <summary>
	/// 向いている方向を設定
	/// </summary>
	/// <param name="radian">向かせる方向(ラジアン)</param>
	void SetRotate(const LWP::Math::Vector3& radian) { radian_ = radian; }
	/// <summary>
	/// 向いている方向を設定
	/// </summary>
	/// <param name="quat">向かせる方向(クォータニオン)</param>
	void SetRotate(const LWP::Math::Quaternion& quat) { quat_ = quat; }
	/// <summary>
	/// クールタイムを設定
	/// </summary>
	/// <param name="time"></param>
	void SetCoolTime() { currentCoolTime_ = coolTime * 60.0f; }

	/// <summary>
	/// ダッシュ攻撃のカプセル判定の終点を設定
	/// </summary>
	/// <param name="end"></param>
	void SetCapsuleEndPos(const LWP::Math::Vector3& end) { capsule_.end = end; }
	/// <summary>
	/// 当たり判定をとるかを設定
	/// </summary>
	/// <param name="isCollision"></param>
	void SetIsCollision(const bool& isCollision) {
		collider_.isActive = isCollision;
		//aabb_.isShowWireFrame = isCollision;
		capsule_.isShowWireFrame = isCollision;
	}
	/// <summary>
	/// 鞘破壊状態かを設定
	/// </summary>
	/// <param name="isBreak"></param>
	void SetIsBreak(const bool& isBreak) { isBreak_ = isBreak; }
#pragma endregion

public:// jsonに保存する値
	// 鞘投げ発動までにかかる時間[秒]
	float throwSwingTime = 0.0f;
	// 鞘を投げて到達するまでの時間[秒]
	float throwTime = 0.2f;
	// 鞘投げの硬直[秒]
	float throwRecoveryTime = 0.0f;

	// 鞘回収発動までにかかる時間[秒]
	float collectSwingTime = 0.0f;
	// 鞘の場所に自機が到着するまでの時間[秒]
	float collectTime = 1.0f;
	// 鞘回収の硬直[秒]
	float collectRecoveryTime = 0.0f;

	// ダッシュ攻撃発動までにかかる時間[秒]
	float dashAttackSwingTime = 0.0f;
	// ダッシュ攻撃時間[秒]
	float dashAttackFinishTime = 0.2f;
	// ダッシュ攻撃の硬直[秒]
	float dashAttackRecoveryTime = 0.0f;

	// 無敵発動までにかかる時間[秒]
	float invinsibleSwingTime = 0.0f;
	// 無敵時間[秒]
	float invinsibleFinishTime = 0.2f;
	// 無敵の硬直[秒]
	float invinsibleRecoveryTime = 0.0f;

	// 鞘を投げた後の移動可能範囲
	float enableMoveRange = 50.0f;

	// 鞘投げの範囲
	LWP::Math::Vector3 throwMovement = { 0,0,10.0f };
	// ダッシュ攻撃の移動量
	LWP::Math::Vector3 dashAttackMovement = { 0,0,10.0f };
	// ダッシュ攻撃の当たり判定の長さ
	LWP::Math::Vector3 dashAttackLength = { 0,0,-10.0f };

	// クールタイム
	float coolTime = 0.0f;

private:// プライベートな変数
	// アクションイベント集
	std::map<int, EventOrder> eventOrders_;

	// 状態遷移
	ISheathSystemState* state_;

	// 鞘のモデル
	LWP::Resource::RigidModel sheathModel_;

	// ダッシュ攻撃判定
	LWP::Object::Collision collider_;
	//LWP::Object::Collider::AABB& aabb_;
	LWP::Object::Collider::Capsule& capsule_;

	// 移動速度
	LWP::Math::Vector3 velocity_;
	// 向いている角度
	LWP::Math::Quaternion quat_ = { 0.0f,0.0f,0.0f,1.0f };
	LWP::Math::Vector3 radian_;

	// クールタイムの経過時間
	float currentCoolTime_;

	// 鞘破壊状態か
	bool isBreak_;
};