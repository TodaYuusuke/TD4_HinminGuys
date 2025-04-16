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
	virtual ~IEnemy() = default;
	//初期化
	virtual void Initialize(Player* player, const Vector3& position) = 0;
	//更新
	virtual void Update() = 0;
	//プレイヤーをセットする関数
	void SetPlayer(Player* player) { player_ = player; }
	//死亡フラグ取得
	bool GetIsDead() const { return isDead_; }
	//座標取得
	const Vector3& GetPosition() { return position_; }
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

protected:

	//モデル
	SkinningModel model_;
	//アニメーション
	Animation animation_;
	//状態
	std::unique_ptr<IEnemyState> state_;

	//プレイヤー情報
	Player* player_;
	//位置
	Vector3 position_{};
	//移動方向
	Vector3 velocity_{};
	//種類
	EnemyType type_;
	//死亡フラグ
	bool isDead_ = false;

};
