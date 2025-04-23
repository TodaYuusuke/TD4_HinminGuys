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
	//初期化
	virtual void Initialize(Player* player, const Vector3& position) = 0;
	//更新
	virtual void Update() = 0;
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
	Player* GetPlayer() { return player_; }
	//プレイヤーの座標取得
	Vector3 GetPlayerPosition();
	//アニメーション切り替え
	void SetAnimation(const std::string& animName, bool isLoop);
	//アニメーション取得
	Animation* GetAnimation() { return &animation_; }
	//状態切り替え
	void SetState(std::unique_ptr<IEnemyState> state);
	//ID取得
	uint32_t GetID() const { return ID_; }
	//反発力取得
	const Vector3& GetRepulsiveForce() const { return repulsiveForce_; }
	//反発力を加算
	void AddRepulsiveForce(const Vector3& force) { repulsiveForce_ += force; }
	//本体当たり判定取得
	LWP::Object::Collider::AABB* GetHitBox() { return &hitBox_; }
	//攻撃当たり判定
	LWP::Object::Collider::AABB* GetAttackHitBox() { return &attackHitBox_; }
	//ロックオンセッター
	void SetIsLocked(bool flag) { isLocked_ = flag; }
	//ロックオンゲッター
	bool GetIsLocked() const { return isLocked_; }
	//デバッグ
	void Debug();
	//プレイヤーからの距離セット
	void SetDistFromPlayer(float dist) { distFromPlayer_ = dist; }
	//プレイヤーからの距離ゲット
	float GetDistFromPlayer() const { return distFromPlayer_; }


	LWP::Object::TransformQuat* GetWorldTF() { return &model_.worldTF; }

protected:

	//モデル
	SkinningModel model_;
	//アニメーション
	Animation animation_;
	//本体当たり判定
	LWP::Object::Collider::AABB hitBox_;
	//攻撃当たり判定
	LWP::Object::Collider::AABB attackHitBox_;
	//状態
	std::unique_ptr<IEnemyState> state_;

	//プレイヤー情報
	Player* player_;
	//互いに距離を取るときの反発力
	Vector3 repulsiveForce_{};
	//種類
	EnemyType type_;
	//全体のID管理
	static uint16_t currentEnemyID_;
	//プレイヤーからの距離
	float distFromPlayer_ = 0.0f;
	//個々のID
	uint16_t ID_;
	//死亡フラグ
	bool isDead_ = false;
	//ロックオンされているか
	bool isLocked_ = false;

};
