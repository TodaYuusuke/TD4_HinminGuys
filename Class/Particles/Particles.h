#pragma once
#include "Adapter.h"
#include "../Player/Systems/Children/Parry/Effect/ParryEffect.h"
#include "../Player/Systems/Children/Evasion/Effect/EvasionEffect.h"
#include "../Player/Systems/Children/Move/Effect/MoveEffect.h"
#include "Common/LargeFlashes.h"
#include "Common/ShortFlashes.h"
#include "Common/Rings.h"
#include "Common/Sparks.h"
#include "Common/DustClouds.h"
#include "Common/Children/FloatParticle.h"
#include "EnemySpawn/EnemySpawnParticles.h"
#include "EnemyDead/EnemyDeadParticles.h"
#include "AttackHit/AttackHitEffect.h"
#include "Crack/CrackEffect.h"


class FollowCamera;
/// <summary>
/// 自機の行動所以で発生するパーティクルの管理クラス
/// </summary>
class Particles {
public:
	enum class MoveEffectType {
		kNone,
		kLeft,
		kRight
	};

public:
	// コンストラクタ
	Particles(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~Particles();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// JSONデータを作成
	/// </summary>
	void CreateJsonData();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

public:// パーティクル生成
	/// <summary>
	/// ジャストパリィ時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateJustParryParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// 弱パリィ時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateGoodParryParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// パリィ時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateParryParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// パリィ時の足元に発生するパーティクル生成開始
	/// </summary>
	/// <param name="pos"></param>
	void CreateFootSparks(LWP::Math::Vector3 pos);
	/// <summary>
	/// 回避時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateEvasionParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// 移動時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateMoveParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// 浮遊パーティクル生成
	/// </summary>
	/// <param name="value"></param>
	/// <param name="pos"></param>
	void CreateFloatParticles(const int& value, const LWP::Math::Vector3& pos) { floatParticle_->Add(value, pos); }
	/// <summary>
	/// 敵出現パーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateEnemySpawnParticles(const LWP::Math::Vector3& pos) { enemySpawnParticles_->Start(true, pos); }
	/// <summary>
	/// 敵出現パーティクル生成終了
	/// </summary>
	void FinishEnemySpawnParticles() { enemySpawnParticles_->Finish(); }
	/// <summary>
	/// 敵死亡パーティクル生成
	/// </summary>
	/// <param name="value"></param>
	/// <param name="pos"></param>
	void CreateEnemyDeadParticles(const int& value, const LWP::Math::Vector3& pos) { enemyDeadParticles_->Add(value, pos); }
	/// <summary>
	/// 攻撃ヒット時のパーティクル生成
	/// </summary>
	/// <param name="value"></param>
	/// <param name="pos"></param>
	void CreateAttackHitEffect(const LWP::Math::Vector3& pos) { attackHitEffect_->Add(pos); }
	/// <summary>
	/// 重攻撃時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateCrackEffect(const LWP::Math::Vector3& pos) { crackEffect_->Add(pos); }
	/// <summary>
	/// 雑魚敵の重攻撃時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateWeakCrackEffect(const LWP::Math::Vector3& pos) { weakCrackEffect_->Add(pos); }

#pragma region Getter
	/// <summary>
	/// 火花パーティクルを取得
	/// </summary>
	/// <returns></returns>
	Sparks* GetSparks() { return sparks_.get(); }
	/// <summary>
	/// 浮遊パーティクルを取得
	/// </summary>
	/// <returns></returns>
	FloatParticle* GetFloatParticles() { return floatParticle_.get(); }
	/// <summary>
	/// 敵出現パーティクル取得
	/// </summary>
	/// <returns></returns>
	EnemySpawnParticles* GetEnemySpawnParticles() { return enemySpawnParticles_.get(); }
	/// <summary>
	/// 敵死亡パーティクル取得
	/// </summary>
	/// <returns></returns>
	EnemyDeadParticles* GetEnemyDeadParticles() { return enemyDeadParticles_.get(); }
	/// <summary>
	/// 攻撃が当たった時のパーティクル取得
	/// </summary>
	/// <returns></returns>
	AttackHitEffect* GetAttackHitEffect() { return attackHitEffect_.get(); }
	/// <summary>
	/// 重攻撃時のパーティクル取得
	/// </summary>
	/// <returns></returns>
	CrackEffect* GetCrackEffect() { return crackEffect_.get(); }
	/// <summary>
	/// 雑魚敵の重攻撃時のパーティクル取得
	/// </summary>
	/// <returns></returns>
	CrackEffect* GetWeakCrackEffect() { return weakCrackEffect_.get(); }
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	Player* player_;
	FollowCamera* followCamera_;

private:
	LWP::Utility::JsonIO json_;

	// パリィ
	std::unique_ptr<ParryEffect> parryEffect_;
	// 回避
	std::unique_ptr<EvasionEffect> evasionEffect_;
	// 移動
	//std::unique_ptr<MoveEffect> moveEffect_;
	std::unique_ptr<DustClouds> dustClouds_;




	std::unique_ptr<LargeFlashes> largeFlashes_;
	std::unique_ptr<ShortFlashes> shortFlashes_;
	std::unique_ptr<Rings> rings_;
	std::unique_ptr<Sparks> sparks_;
	// 浮遊パーティクル
	std::unique_ptr<FloatParticle> floatParticle_;
	std::unique_ptr<EnemySpawnParticles> enemySpawnParticles_;
	std::unique_ptr<EnemyDeadParticles> enemyDeadParticles_;
	std::unique_ptr<AttackHitEffect> attackHitEffect_;
	std::unique_ptr<CrackEffect> crackEffect_;
	// 弱めの攻撃
	std::unique_ptr<CrackEffect> weakCrackEffect_;



	// パーティクル生成座標(デバッグ用)
	LWP::Math::Vector3 debugEmitterPos_;

	bool isPreJustParry_ = false;
	bool isPreGoodParry_ = false;
};

