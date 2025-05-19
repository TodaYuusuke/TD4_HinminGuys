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

	/// <summary>
	/// デバッグ用のタブを表示(Debug時のみ)
	/// </summary>
	void DebugGUI();
	
private:
	/// <summary>
	/// 自機機能を全て生成
	/// </summary>
	void CreateSystems();

public:// Getter,Setter
#pragma region Getter
	/// <summary>
	/// 各機能をまとめているクラスのアドレスを取得
	/// </summary>
	/// <returns></returns>
	SystemManager* GetSystemManager() { return systemManager_.get(); }
	/// <summary>
	/// 自機のTransformQuatを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat* GetWorldTF() { return &model_.worldTF; }
	/// <summary>
	/// アニメーション情報を取得
	/// </summary>
	/// <returns></returns>
	LWP::Resource::Animation* GetAnimation() { return &animation_; }
	/// <summary>
	/// 自機の座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3* GetModelPos() { return &model_.worldTF.translation; }
	/// <summary>
	/// 自機の速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return systemManager_->GetVelocity(); }
	/// <summary>
	/// 自機の角度を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetQuat() { return systemManager_->GetRotate(); }
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

	/// <summary>
	/// アニメーションを開始
	/// </summary>
	/// <param name="animName">再生するアニメーション名</param>
	/// <param name="transitionTime">モーションの遷移にかかる時間(0.0f以上)</param>
	/// <param name="startTime">開始時間(0.0f ~ 1.0f)</param>
	void StartAnimation(const std::string& animName, const float& transitionTime, const float& startTime) {
		// アニメーションが再生されているなら早期リターン
		if (animation_.GetPlaying(animName)) { return; }
		animation_.Play(animName, transitionTime, startTime); 
	}
	/// <summary>
	/// アニメーションを初期化
	/// </summary>
	void ResetAnimation() { 
		animation_.Init();
		animation_.Pause();
		animation_.Loop(false);
		animation_.GetPlayBackSpeed() = 1.0f;
	}
	/// <summary>
	/// アニメーションの再生速度を設定
	/// </summary>
	/// <param name="playSpeed"></param>
	void SetAnimationPlaySpeed(const float& playSpeed) { animation_.GetPlayBackSpeed() = playSpeed; }
	/// <summary>
	/// アニメーションをループするかを設定
	/// </summary>
	/// <param name="isLoop"></param>
	void SetIsLoopAnimation(const bool& isLoop) { animation_.Loop(isLoop); }
#pragma endregion

private:// 外部からポインタをもらう変数
	// 敵の管理クラス
	EnemyManager* enemyManager_;
	// 追従カメラ
	FollowCamera* followCamera_;

private:
	// 機能まとめ
	std::unique_ptr<SystemManager> systemManager_;
};