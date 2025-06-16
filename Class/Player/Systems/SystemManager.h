#pragma once
#include "../Adapter/Adapter.h"
#include "Children/Move/Move.h"
#include "Children/Parry/Parry.h"
#include "Children/Attack/Attack.h"
#include "Children/LockOn/LockOn.h"
#include "Children/Evasion/Evasion.h"
#include "Children/Sheath/Sheath.h"
#include "Children/Damage/DamageResponse.h"
#include "../Command/InputHandler.h"
#include "../../ComboEditor/ComboTree.h"
#include "../../Components/CoolTimer.h"
#include <memory>
#include <functional>

class Player;
class EnemyManager;
class FollowCamera;

enum class InputState {
	kMove,
	kAttack,
	kParry,
	kLockOn,
	kEvasion,
	kSheath
};
struct SuccessParryData {
	bool isActive;
	Vector3 targetPos;
};
class SystemManager {
public:
	// コンストラクタ
	SystemManager(Player* player, EnemyManager* enemyManager, FollowCamera* followCamera, LWP::Object::Camera* camera);
	// デストラクタ
	~SystemManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 全ての数値をリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// ImGuiによるパラメータを表示
	/// </summary>
	void DebugGUI();

	/// <summary>
	/// ImGuiによるコンボのGUI表示
	/// </summary>
	void DebugComboGUI() { comboTree_->DebugGUI(); }
	/// <summary>
	/// コンボのリセット
	/// </summary>
	void ComboReset() {
		comboTree_->ResetCombo();
	}

	/// <summary>
	/// ダメージリアクション開始
	/// </summary>
	void StartDamageResponse() { CreateDamageResponseSystem(currentSystem_); }

private:
	/// <summary>
	/// 移動機能を生成
	/// </summary>
	void CreateMoveSystem(ISystem*& system);
	/// <summary>
	/// 攻撃機能を生成
	/// </summary>
	void CreateAttackSystem(ISystem*& system);
	/// <summary>
	/// 回避機能を生成
	/// </summary>
	void CreateEvasionSystem(ISystem*& system);
	/// <summary>
	/// パリィ機能を生成
	/// </summary>
	void CreateParrySystem(ISystem*& system);
	/// <summary>
	/// 鞘機能を生成
	/// </summary>
	void CreateSheathSystem(ISystem*& system);
	///// <summary>
	///// ダメージ反応機能を生成
	///// </summary>
	void CreateDamageResponseSystem(ISystem*& system);

	/// <summary>
	/// 現在のシステムの更新処理
	/// </summary>
	void CurrentSystemUpdate();
	/// <summary>
	/// 現在使用する機能の切り替え
	/// </summary>
	void SwitchCurrentSystem();

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// ロックオン機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	LockOn* GetLockOnSystem() { return lockOnSystem_.get(); }
	/// <summary>
	/// 鞘機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	Sheath* GetSheathSystem() { return sheathSystem_.get(); }

	/// <summary>
	/// クールタイマーを取得
	/// </summary>
	/// <returns></returns>
	CoolTimer* GetCoolTimer() { return coolTimer_; }
	/// <summary>
	/// コンボツリーを取得
	/// </summary>
	/// <returns></returns>
	ComboTree* GetComboTree() { return comboTree_; }
	/// <summary>
	/// パリィの当たり判定を取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::Collision& GetParryCollision() { return parryCollision_; }
	/// <summary>
	/// 鞘攻撃の当たり判定を取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::Collision& GetSheathAttackCollision() { return sheathCollision_; }
	/// <summary>
	/// 鞘攻撃のカプセル情報を取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::Collider::Capsule& GetSheathAttackCapsule() { return sheathAttackCapsule_; }

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 角度を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	Quaternion GetQuat() { return quat_; }
	/// <summary>
	/// 角度を取得(オイラー角)
	/// </summary>
	/// <returns></returns>
	Vector3 GetRadian() { return radian_; }
	/// <summary>
	/// 現在起動しているシステム
	/// </summary>
	/// <returns></returns>
	ISystem* GetCurrentSystem() { return currentSystem_; }
	/// <summary>
	/// パリィ成功時の情報を取得
	/// </summary>
	/// <returns></returns>
	SuccessParryData GetSuccessParryData() { return successParryData_; }
	/// <summary>
	/// 無敵時間を取得
	/// </summary>
	/// <param name="invinsibleTime"></param>
	float GetInvisibleTime() { return invinsibleTime_; }
	/// <summary>
	/// ダッシュ可能かを取得
	/// </summary>
	bool GetIsEnableDash() { return isEnableDash_; }
	/// <summary>
	/// パリィが成功したかを取得
	/// </summary>
	bool GetIsSuccessParry() { return successParryData_.isActive; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// パリィの当たった時の処理の関数ポインタを設定
	/// </summary>
	void SetParryOnHitFunc(LWP::Object::Collision::OnHitFunction parryOnHitFunc) { parryCollision_.stayLambda = parryOnHitFunc; }
	/// <summary>
	/// 鞘攻撃に当たった時の処理の関数ポインタを設定
	/// </summary>
	void SetSheathAttackOnHitFunc(LWP::Object::Collision::OnHitFunction sheathAttackOnHitFunc) { sheathCollision_.stayLambda = sheathAttackOnHitFunc; }

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
	/// パリィされた対象の座標を設定
	/// </summary>
	void SetOnParryTargetPos(const LWP::Math::Vector3& pos) { successParryData_.targetPos = pos; }
	/// <summary>
	/// パリィのクールタイムを設定
	/// </summary>
	/// <param name="coolTime">クールタイム[秒]</param>
	void SetParryCoolTime(const float& coolTime) { coolTimer_->SetParryCoolTime(coolTime); }
	/// <summary>
	/// 回避のクールタイムを設定
	/// </summary>
	/// <param name="coolTime">クールタイム[秒]</param>
	void SetEvasionCoolTime(const float& coolTime) { coolTimer_->SetEvasionCoolTime(coolTime); }
	/// <summary>
	/// 鞘のクールタイムを設定
	/// </summary>
	/// <param name="coolTime">クールタイム[秒]</param>
	void SetSheathCoolTime(const float& coolTime) { coolTimer_->SetSheathCoolTime(coolTime); }
	/// <summary>
	/// 無敵時間を設定
	/// </summary>
	/// <param name="invinsibleTime"></param>
	void SetInvisibleTime(const float& invinsibleTime) { invinsibleTime_ = invinsibleTime * 60.0f; }
	/// <summary>
	/// ダッシュ可能かを設定
	/// </summary>
	/// <param name="isEnableDash"></param>
	void SetIsEnableDash(const bool& isEnableDash) { isEnableDash_ = isEnableDash; }
	/// <summary>
	/// パリィが成功したかを設定
	/// </summary>
	/// <param name="isSuccessParry"></param>
	void SetIsSuccessParry(const bool& isSuccessParry) { successParryData_.isActive = isSuccessParry; }
#pragma endregion

private:// 外部からポインタをもらう変数
	Player* player_;
	// 敵の管理クラス
	EnemyManager* enemyManager_;
	// 追従カメラ
	FollowCamera* followCamera_;
	// シーンで使用しているカメラ
	LWP::Object::Camera* pCamera_;
	//　キー入力
	InputHandler* inputHandler_;

private:
#pragma region 常に作動させる機能
	// ロックオン機能
	std::unique_ptr<LockOn> lockOnSystem_;
	// 鞘機能
	std::unique_ptr<Sheath> sheathSystem_;
#pragma endregion

#pragma region JSONファイル作成のために一度作る機能
	// パリィ機能
	std::unique_ptr<Parry> parrySystem_;
	// 攻撃機能
	std::unique_ptr<Attack> attackSystem_;
	// 移動機能
	std::unique_ptr<Move> moveSystem_;
	// 回避機能
	std::unique_ptr<Evasion> evasionSystem_;
	// 被弾機能
	std::unique_ptr<DamageResponse> damageResponse_;
#pragma endregion

	// 機能クラスをまとめた変数
	std::vector<ISystem*> systems_;

	// 現在稼働しているシステム
	ISystem* currentSystem_;
	// 使用している機能
	SystemState systemState_;

	// クールタイマー
	CoolTimer* coolTimer_;

	// コンボ攻撃用クラス
	ComboTree* comboTree_;

#pragma region 当たり判定に関する変数
	// 攻撃の当たり判定の内容
	LWP::Object::Collision::OnHitFunction attackOnHitFunc_;
	// パリィ判定
	LWP::Object::Collision parryCollision_;
	LWP::Object::Collider::AABB& parryAABB_;
	// ダッシュ攻撃判定
	LWP::Object::Collision sheathCollision_;
	LWP::Object::Collider::Capsule& sheathAttackCapsule_;
#pragma endregion

	// 速度
	LWP::Math::Vector3 velocity_;
	// 角度
	LWP::Math::Vector3 radian_;
	LWP::Math::Quaternion quat_;

	// パリィ成功時の情報
	SuccessParryData successParryData_;

	// ノックバックの速度
	Vector3 knockBackVel_;
	// 無敵時間
	float invinsibleTime_;
	// クールタイム
	float coolTime_;

	// 鞘ゲージがなくなっているか
	bool isNoneSheathGauge_;
	// ダッシュ可能か
	bool isEnableDash_;
};