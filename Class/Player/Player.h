#pragma once
#include "ICharacter.h"
#include "Systems/Children/Move.h"
#include "Systems/Children/Parry.h"
#include "Systems/Children/Attack.h"
#include "Systems/Children/LockOn.h"
#include "Systems/SystemManager.h"
#include <memory>

class IEnemy;
class EnemyManager;
class FollowCamera;
class Player : public ICharacter {
public:
	// コンストラクタ
	Player(LWP::Object::Camera* camera, EnemyManager* enemyManager, FollowCamera* followCamera);
	// デストラクタ
	~Player() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 全ての数値をリセットする
	/// </summary>
	void Reset();

private:
	/// <summary>
	/// 自機機能を全て生成
	/// </summary>
	void CreateSystems();

	/// <summary>
	/// デバッグ用のタブを表示(Debug時のみ)
	/// </summary>
	void DebugGUI();

public:// Getter,Setter
#pragma region Getter
	/// <summary>
	/// 自機の座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3* GetModelPos() { return &model_.worldTF.translation; }
	/// <summary>
	/// 自機のTransformQuatを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat* GetWorldTF() { return &model_.worldTF; }
	/// <summary>
	/// 攻撃機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	Attack* GetAttackSystem() { return systemManager_->GetAttackSystem(); }
	/// <summary>
	/// パリィ機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	Parry* GetParrySystem() { return systemManager_->GetParrySystem(); }
	/// <summary>
	/// ロックオン機能のアドレスを取得
	/// </summary>
	/// <returns></returns>
	LockOn* GetLockOnSystem() { return systemManager_->GetLockOnSystem(); }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// シーンで使用しているカメラのポインタを設定
	/// </summary>
	/// <param name="camera">カメラのアドレス</param>
	void SetCamera(LWP::Object::Camera* camera) { pCamera_ = camera; }

	/// <summary>
	/// 敵の管理クラスを設定
	/// </summary>
	/// <param name="enemyManager">敵の管理クラスのポインタ</param>
	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }
#pragma endregion

private:// 外部からポインタをもらう変数
	// 敵の管理クラス
	EnemyManager* enemyManager_;
	// 追従カメラ
	FollowCamera* followCamera_;

private:
	//// 移動機能
	//std::unique_ptr<Move> moveSystem_;
	//// パリィ機能
	//std::unique_ptr<Parry> parrySystem_;
	//// 攻撃機能
	//std::unique_ptr<Attack> attackSystem_;
	//// ロックオン機能
	//std::unique_ptr<LockOn> lockOnSystem_;

	std::unique_ptr<SystemManager> systemManager_;
};