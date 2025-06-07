#pragma once
#include "../Adapter/Adapter.h"
#include "Children/Move.h"
#include "Children/Parry.h"
#include "Children/Attack.h"
#include "Children/LockOn.h"
#include "Children/Evasion.h"
#include "Children/Sheath.h"
#include "Children/DamageResponse.h"
#include "../Animator/PlayerAnimator.h"
#include "../Command/InputHandler.h"
#include "../../ComboEditor/ComboTree.h"
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
		comboTree_->SetIsRecept(false);
	}

public:
	/// <summary>
	/// 移動機能を生成
	/// </summary>
	void CreateMoveSystem();
	/// <summary>
	/// 攻撃機能を生成
	/// </summary>
	void CreateAttackSystem();
	/// <summary>
	/// 回避機能を生成
	/// </summary>
	void CreateEvasionSystem();
	/// <summary>
	/// パリィ機能を生成
	/// </summary>
	void CreateParrySystem();
	/// <summary>
	/// 鞘機能を生成
	/// </summary>
	void CreateSheathSystem();
	///// <summary>
	///// ダメージ反応機能を生成
	///// </summary>
	//void CreateDamageResponseSystem();
	///// <summary>
	///// ロックオン機能を生成
	///// </summary>
	//void CreateLockOnSystem();

	/// <summary>
	/// 機能の切り替え
	/// </summary>
	void SwitchCurrentSystem();

public:// Getter, Setter
#pragma region Getter
	ComboTree* GetComboTree() { return comboTree_; }
	/// <summary>
	/// ロックオン機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	LockOn* GetLockOnSystem() { return lockOnSystem_.get(); }
	///// <summary>
	///// パリィ機能のアドレスを取得
	///// </summary>
	///// <returns></returns>
	//Parry* GetParrySystem() { return parrySystem_.get(); }
	///// <summary>
	///// 攻撃機能のアドレスを取得
	///// </summary>
	///// <returns></returns>
	//Attack* GetAttackSystem() { return attackSystem_.get(); }
	///// <summary>
	///// 移動機能のアドレスを取得
	///// </summary>
	///// <returns></returns>
	//Move* GetMoveSystem() { return moveSystem_.get(); }
	///// <summary>
	///// 回避機能のアドレスを取得
	///// </summary>
	///// <returns></returns>
	//Evasion* GetEvasionSystem() { return evasionSystem_.get(); }
	///// <summary>
	///// 鞘機能のアドレスを取得
	///// </summary>
	///// <returns></returns>
	//Sheath* GetSheathSystem() { return sheathSystem_.get(); }
	/// <summary>
	/// 被弾機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	DamageResponse* GetDamageResponseSystem() { return damageResponseSystem_.get(); }
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
	/// ダッシュ可能かを取得
	/// </summary>
	bool GetIsEnableDash() { return isEnableDash_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 現在のsystemのリセット関数を設定
	/// </summary>
	/// <param name="resetSystemFunc"></param>
	void SetResetSystemFunc(std::function<void()> resetSystemFunc) { resetSystemFunc_ = resetSystemFunc; }
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
	/// 入力状態を設定
	/// </summary>
	/// <param name="inputState"></param>
	void SetInputState(InputState inputState) { inputState_ = inputState; }
	/// <summary>
	/// ダッシュ可能かを設定
	/// </summary>
	/// <param name="isEnableDash"></param>
	void SetIsEnableDash(const bool& isEnableDash) { isEnableDash_ = isEnableDash; }
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
	// ロックオン機能
	std::unique_ptr<LockOn> lockOnSystem_;
	// パリィ機能
	std::unique_ptr<Parry> parrySystem_;
	// 攻撃機能
	std::unique_ptr<Attack> attackSystem_;
	// コンボ攻撃用クラス
	ComboTree* comboTree_;
	// 当たり判定の内容
	LWP::Object::Collision::OnHitFunction onCollision_;
	// 移動機能
	std::unique_ptr<Move> moveSystem_;
	// 回避機能
	std::unique_ptr<Evasion> evasionSystem_;
	// 鞘機能
	std::unique_ptr<Sheath> sheathSystem_;
	// 被弾機能
	std::unique_ptr<DamageResponse> damageResponseSystem_;

	// 機能クラスをまとめた変数
	std::vector<ISystem*> systems_;

	// 現在稼働しているシステム
	ISystem* currentSystem_;
	// 並列で稼働するシステム(鞘システムのような状態で使うことを想定)
	ISystem* secondSystem_;

	// systemのリセット関数を格納
	std::function<void()> resetSystemFunc_;

	// 速度
	LWP::Math::Vector3 velocity_;
	// 角度
	LWP::Math::Vector3 radian_;
	LWP::Math::Quaternion quat_;

	// ノックバックの速度
	Vector3 knockBackVel_;
	// 無敵時間
	float invinsibleTime_;
	// クールタイム
	float coolTime_;

	// 入力状態
	InputState inputState_;
	SystemState systemState_;
	// ひとつ前に使用していた機能
	SystemState preSystemState_;

	// 鞘ゲージがなくなっているか
	bool isNoneSheathGauge_;
	// ダッシュ可能か
	bool isEnableDash_;
};