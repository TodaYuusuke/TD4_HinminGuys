#pragma once
#include "../Adapter/Adapter.h"

using namespace LWP::Math;
using namespace LWP::Resource;

class Player;
class EnemyManager;

/// <summary>
/// 敵の種類
/// </summary>
enum class EnemyType {
	kNormal, //雑魚敵
	kBoss, //ボス敵

	kMax
};

/// <summary>
/// 敵の攻撃パラメータ
/// </summary>
struct EnemyAttackParameter {
	float attackValue = 10.0f; //攻撃力
	float knockbackValue = 0.5f; //ノックバック
};

/// <summary>
/// 敵の全体パラメータ
/// </summary>
struct EnemyParameter {
	float hp = 10.0f; //体力
	EnemyAttackParameter attackParameter; //攻撃パラメータ
};

/// <summary>
/// 敵の基底クラス
/// </summary>
class IEnemy
{
public:
	IEnemy();
	virtual ~IEnemy();

	//デバッグ
	virtual void DebugGUI() = 0;
	//敵の全種類ネームタグ
	static std::array<const char*, int(EnemyType::kMax)> enemyTypeName;

	//初期化
	virtual void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera, 
		EnemyManager* manager) = 0;
	//更新
	virtual void Update();
	//プレイヤーをセットする関数
	void SetPlayer(Player* player) { player_ = player; }
	//死亡フラグ取得
	bool GetIsDead() const { return isDead_; }
	//座標取得
	const Vector3& GetPosition() const { return model_.worldTF.translation; }
	//座標セット
	void SetPosition(const Vector3& position) { model_.worldTF.translation = position; }
	//回転取得
	const Quaternion& GetRotation() const { return model_.worldTF.rotation; }
	//回転セット
	void SetRotation(const Quaternion& rotation) { model_.worldTF.rotation = rotation; }
	//プレイヤー取得
	Player* GetPlayerPtr() { return player_; }
	//マネージャー取得
	EnemyManager* GetManagerPtr() { return enemyManager_; }
	//プレイヤーの座標取得
	Vector3 GetPlayerPosition();
	//アニメーション切り替え
	void SetAnimation(const std::string& animName, bool isLoop, float speed = 1.0f);
	//アニメーション取得
	Animation* GetAnimation() { return &animation_; }
	//ID取得
	uint32_t GetID() const { return ID_; }
	//反発力取得
	const Vector3& GetRepulsiveForce() const { return repulsiveForce_; }
	//反発力を加算
	void AddRepulsiveForce(const Vector3& force) { repulsiveForce_ += force; }
	//ロックオンセッター
	void SetIsLocked(bool flag) { isLocked_ = flag; }
	//ロックオンゲッター
	bool GetIsLocked() const { return isLocked_; }
	//プレイヤーからの距離セット
	void SetDistFromPlayer(float dist) { distFromPlayer_ = dist; }
	//プレイヤーからの距離ゲット
	float GetDistFromPlayer() const { return distFromPlayer_; }
	//プレイヤーの方向に回転
	void RotateTowardsPlayer();
	//攻撃開始
	void BeginAttack() { isAttack_ = true; }
	//攻撃終了
	void EndAttack() { isAttack_ = false; }
	//攻撃中フラグ取得
	bool GetIsAttack() const { return isAttack_; }
	//攻撃フラグを強制終了(外部からの呼び出し用)
	void ResetAttack() { isAttack_ = false; }
	//攻撃態勢状態取得
	bool GetIsAttackPhase() const { return isAttackPhase_; }
	//攻撃態勢状態セット
	void SetIsAttackPhase(bool flag) { isAttackPhase_ = flag; }
	//最大攻撃態勢人数取得
	static uint16_t GetMaxAttackCount() { return maxAttackCount_; }
	//近接カウントセット
	void SetClosenessCount(uint16_t count) { closenessCount_ = count; }
	//近接カウントゲット
	uint16_t GetClosenessCount() const { return closenessCount_; }

	LWP::Object::TransformQuat* GetWorldTF() { return &model_.worldTF; }

	//コライダー名取得
	const std::string& GetColliderName() const { return collider_.name; }
	//攻撃パラメータ取得
	const EnemyAttackParameter& GetAttackParameter() const { return parameter_.attackParameter; }
	//刀モデル取得
	SkinningModel& GetModel() { return model_; }
	//刀モデル取得
	SkinningModel& GetSwordModel() { return swordModel_; }
	//刀当たり判定取得
	LWP::Object::Collision& GetSwordCollider() { return swordCollider_; }
	//パリィエフェクト開始
	void StartParryEffect();
	//パリィエフェクト中かどうか
	bool GetIsStartParryEffect() const { return isStartParryEffect_; }
	//ノックバック取得
	Vector3& GetKnockBackVelocity() { return knockBackVelocity_; }
	//ノックバックセット
	void SetKnockBackVelocity(const Vector3& velocity) { knockBackVelocity_ = velocity; }
	//パリィエフェクトが終わった瞬間だけ取得
	bool IsExitParryEffect() { return not isStartParryEffect_ and preIsStartParryEffect_; }

protected:

	//ダメージを与える
	void TakeDamage(const float& damageValue, const float& multiply = 1.0f) {
		parameter_.hp -= damageValue * multiply;
	}

	//ノックバックの力をセットする
	void SetKnockBackValue(const float& knockBackValue);

	//刀のコライダー生成
	void CreateSwordCollider();
	//パリィエフェクト更新
	void UpdateParryEffect();
	//座標変換
	LWP::Math::Vector3 CoordTransform(const LWP::Math::Vector3& vector, const LWP::Math::Matrix4x4& matrix);

protected:

	//パリィエフェクトの画像数
	static const int32_t kMaxParryEffect_ = 2;
	//カメラのポインタ
	LWP::Object::Camera* camera_ = nullptr;
	//モデル
	SkinningModel model_;
	// 刀モデル
	SkinningModel swordModel_;
	//アニメーション
	Animation animation_;
	//本体当たり判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;
	// 刀コライダー
	LWP::Object::Collision swordCollider_;
	LWP::Object::Collider::Capsule& capsule_;
	//パリィエフェクト画像
	std::array<LWP::Primitive::Sprite, kMaxParryEffect_> parryEffectSprite_;
	
	//プレイヤー情報
	Player* player_;
	//敵全体から情報を取るためのポインタ
	EnemyManager* enemyManager_;
	//敵個別のパラメータ
	EnemyParameter parameter_;
	//互いに距離を取るときの反発力
	Vector3 repulsiveForce_{};
	//ノックバック力
	Vector3 knockBackVelocity_{};
	//種類
	EnemyType type_;
	//プレイヤーからの距離
	float distFromPlayer_ = 0.0f;
	//パリィエフェクトの現在の時間
	float parryEffectTime_ = 0.0f;
	//パリィエフェクトの最大時間
	float maxParryEffectTime_ = 0.5f;
	//全体のID管理
	static uint16_t currentEnemyID_;
	//攻撃態勢最大人数
	static uint16_t maxAttackCount_;
	//個々のID
	uint16_t ID_;
	//距離の近さを示す変数。小さいほど近い
	uint16_t closenessCount_ = 0;
	//死亡フラグ
	bool isDead_ = false;
	//ロックオンされているか
	bool isLocked_ = false;
	//攻撃態勢に入ったかどうか
	bool isAttackPhase_ = false;
	//攻撃中かどうか
	bool isAttack_ = false;
	//パリィエフェクト中かどうか
	bool isStartParryEffect_ = false;
	//前フレームのパリィエフェクトフラグ
	bool preIsStartParryEffect_ = false;

};
