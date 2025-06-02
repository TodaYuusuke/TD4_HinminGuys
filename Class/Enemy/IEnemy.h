#pragma once
#include "../Adapter/Adapter.h"

using namespace LWP::Math;
using namespace LWP::Resource;

class Player;

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

//前方宣言
class IEnemyState;

/// <summary>
/// 敵の基底クラス
/// </summary>
class IEnemy
{
public:
	IEnemy();
	virtual ~IEnemy();

	//敵の全種類ネームタグ
	static std::array<const char*, int(EnemyType::kMax)> enemyTypeName;

	//初期化
	virtual void Initialize(Player* player, const Vector3& position) = 0;
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
	//プレイヤーの座標取得
	Vector3 GetPlayerPosition();
	//アニメーション切り替え
	void SetAnimation(const std::string& animName, bool isLoop, float speed = 1.0f);
	//アニメーション取得
	Animation* GetAnimation() { return &animation_; }
	//状態切り替え
	void SetState(IEnemyState* state);
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
	//デバッグ
	void DebugGUI();
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
	static void ResetAttack() { isAttack_ = false; }
	//攻撃態勢カウント増加
	void AddAttackCount() { currentAttackCount_++; }
	//攻撃態勢カウント減少
	void SubAttackCount() { currentAttackCount_--; }
	//現在の攻撃態勢人数取得
	static uint16_t GetCurrentAttackCount() { return currentAttackCount_; }
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

protected:

	//ダメージを与える
	void TakeDamage(const float& damageValue, const float& multiply = 1.0f) {
		parameter_.hp -= damageValue * multiply;
	}

	//刀のコライダー生成
	void CreateSwordCollider();

protected:

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
	//状態
	IEnemyState* state_;

	//プレイヤー情報
	Player* player_;
	//敵個別のパラメータ
	EnemyParameter parameter_;
	//互いに距離を取るときの反発力
	Vector3 repulsiveForce_{};
	//種類
	EnemyType type_;
	//プレイヤーからの距離
	float distFromPlayer_ = 0.0f;
	//全体のID管理
	static uint16_t currentEnemyID_;
	//攻撃態勢最大人数
	static uint16_t maxAttackCount_;
	//現在攻撃態勢に入っている人数
	static uint16_t currentAttackCount_;
	//個々のID
	uint16_t ID_;
	//距離の近さを示す変数。小さいほど近い
	uint16_t closenessCount_ = 0;
	//死亡フラグ
	bool isDead_ = false;
	//ロックオンされているか
	bool isLocked_ = false;
	//攻撃中かどうか
	static bool isAttack_;

};
