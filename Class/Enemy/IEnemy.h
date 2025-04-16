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
/// 敵の基底クラス
/// </summary>
class IEnemy
{
public:
	virtual ~IEnemy() = default;
	//初期化
	virtual void Initialize(Player* player, const Vector3& position) = 0;
	//更新
	virtual void Update() = 0;
	//プレイヤーをセットする関数
	void SetPlayer(Player* player) { player_ = player; }
	//死亡フラグ取得
	bool GetIsDead() const { return isDead_; }
	//プレイヤー取得
	Player* GetPlayer() { return player_; }
	//プレイヤーの座標取得
	Vector3 GetPlayerPosition() const { return playerPosition_; }

protected:

	//モデル
	RigidModel model_;
	//プレイヤー情報
	Player* player_;
	//プレイヤー座標(仮)
	Vector3 playerPosition_{};
	//位置
	Vector3 position_{};
	//移動方向
	Vector3 velocity_{};
	//種類
	EnemyType type_;
	//速度
	float speed_ = 1.0f;
	//死亡フラグ
	bool isDead_ = false;

};
