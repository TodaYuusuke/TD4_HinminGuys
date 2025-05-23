#pragma once
#include "ICharacter.h"
#include "Systems/Children/Move.h"
#include "Systems/Children/Parry.h"
#include "Systems/Children/Attack.h"
#include "Systems/Children/LockOn.h"
#include "Systems/SystemManager.h"
#include "Gauge/HP/HP.h"
#include "Gauge/Sheath/SheathGauge.h"
#include "Command/InputHandler.h"
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

	/// <summary>
	/// ImGuiによるコンボのGUI表示
	/// </summary>
	void DebugComboGUI() { systemManager_->DebugComboGUI(); }

private:
	/// <summary>
	/// 自機機能を全て生成
	/// </summary>
	void CreateSystems();

	/// <summary>
	/// 当たり判定を作成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// 移動制限
	/// </summary>
	void LimitMoveArea();

public:// Getter,Setter
#pragma region Getter
	/// <summary>
	/// 追従カメラの情報を取得
	/// </summary>
	/// <returns></returns>
	FollowCamera* GetFollowCamera() { return followCamera_; }
	/// <summary>
	/// 各機能をまとめているクラスのアドレスを取得
	/// </summary>
	/// <returns></returns>
	SystemManager* GetSystemManager() { return systemManager_.get(); }

	InputHandler* GetInputHandler() { return inputHandler_; }
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
	/// モデルを取得
	/// </summary>
	/// <returns></returns>
	LWP::Resource::SkinningModel* GetModel() { return &model_; }
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

	void SetInputHandler(InputHandler* inputHandler) { inputHandler_ = inputHandler; }

	/// <summary>
	/// アニメーションを開始
	/// </summary>
	/// <param name="animName">再生するアニメーション名</param>
	/// <param name="transitionTime">モーションの遷移にかかる時間(0.0f以上)</param>
	/// <param name="startTime">開始時間(0.0f ~ 1.0f)</param>
	void StartAnimation(const std::string& animName, const float& transitionTime, const float& startTime, LWP::Resource::Animation::TrackType type = LWP::Resource::Animation::TrackType::Main) {
		// アニメーションが再生されているなら早期リターン
		//if (animation_.GetPlaying(animName)) { return; }
		animation_.Play(animName, transitionTime, startTime, type);
	}
	/// <summary>
	/// アニメーションを初期化
	/// </summary>
	void ResetAnimation() { animation_.Loop(false); }
	/// <summary>
	/// ブレンドされているアニメーションを停止
	/// </summary>
	void StopAnimation(LWP::Resource::Animation::TrackType type = LWP::Resource::Animation::TrackType::Main) { animation_.Stop(type); }
	/// <summary>
	/// アニメーションの再生速度を設定
	/// </summary>
	/// <param name="playSpeed"></param>
	void SetAnimationPlaySpeed(const float& playSpeed) { /*animation_.playbackSpeed = */playSpeed; }
	/// <summary>
	/// アニメーションをループするかを設定
	/// </summary>
	/// <param name="isLoop"></param>
	void SetIsLoopAnimation(const bool& isLoop, LWP::Resource::Animation::TrackType type = LWP::Resource::Animation::TrackType::Main) { animation_.Loop(isLoop, type); }
	void SetPos(LWP::Math::Vector3 pos) { model_.worldTF.translation = pos; }
#pragma endregion

private:// 外部からポインタをもらう変数
	// 敵の管理クラス
	EnemyManager* enemyManager_;
	// 追従カメラ
	FollowCamera* followCamera_;
	InputHandler* inputHandler_;

private:
	// 体の判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;

	// 機能まとめ
	std::unique_ptr<SystemManager> systemManager_;

	// HP
	HP hp_;

	// 鞘ゲージ
	SheathGauge sheathGauge_;
};