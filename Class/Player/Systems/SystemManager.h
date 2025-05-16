#pragma once
#include "../Adapter/Adapter.h"
#include "Children/Move.h"
#include "Children/Parry.h"
#include "Children/Attack.h"
#include "Children/LockOn.h"
#include "Children/Evasion.h"
#include "Children/Sheath.h"
#include "../Animator/PlayerAnimator.h"
#include <memory>

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
	~SystemManager() = default;

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

private:
	/// <summary>
	/// 移動入力が可能な状態なら自機に速度を加算
	/// </summary>
	void EnableInputMoveState();

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// ロックオン機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	LockOn* GetLockOnSystem() { return lockOnSystem_.get(); }
	/// <summary>
	/// パリィ機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	Parry* GetParrySystem() { return parrySystem_.get(); }
	/// <summary>
	/// 攻撃機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	Attack* GetAttackSystem() { return attackSystem_.get(); }
	/// <summary>
	/// 移動機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	Move* GetMoveSystem() { return moveSystem_.get(); }
	/// <summary>
	/// 回避機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	Evasion* GetEvasionSystem() { return evasionSystem_.get(); }
	/// <summary>
	/// 鞘機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	Sheath* GetSheathSystem() { return sheathSystem_.get(); }

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 角度を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	Quaternion GetRotate() { return rotate_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 入力状態を設定
	/// </summary>
	/// <param name="inputState"></param>
	void SetInputState(InputState inputState) { inputState_ = inputState; }
#pragma endregion

private:// 外部からポインタをもらう変数
	Player* player_;
	// 敵の管理クラス
	EnemyManager* enemyManager_;
	// 追従カメラ
	FollowCamera* followCamera_;
	// シーンで使用しているカメラ
	LWP::Object::Camera* pCamera_;

private:
	// ロックオン機能
	std::unique_ptr<LockOn> lockOnSystem_;
	// パリィ機能
	std::unique_ptr<Parry> parrySystem_;
	// 攻撃機能
	std::unique_ptr<Attack> attackSystem_;
	// 移動機能
	std::unique_ptr<Move> moveSystem_;
	// 回避機能
	std::unique_ptr<Evasion> evasionSystem_;
	// 鞘
	std::unique_ptr<Sheath> sheathSystem_;

	// 機能クラスをまとめた変数
	std::vector<ISystem*> systems_;

	// アニメーションの遷移を管理するクラス
	PlayerAnimator animator_;

	// 速度
	LWP::Math::Vector3 velocity_;

	// 角度
	LWP::Math::Vector3 radian_;
	LWP::Math::Quaternion rotate_;

	// 入力状態
	InputState inputState_;
};