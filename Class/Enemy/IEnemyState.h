#pragma once
#include "../Adapter/Adapter.h"

class IEnemy;

/// <summary>
/// 状態一覧
/// </summary>
enum class States {
	kNormalIdle,
	kNormalMove,
	kNormalAttack,
	kSpacing,
	kFollowing,
	kWaitingForAttack,
	kMax,
};

/// <summary>
/// 敵の行動パターン基底クラス
/// </summary>
class IEnemyState
{
public:
	virtual ~IEnemyState() = default;
	//初期化
	virtual void Initialize(IEnemy* enemy) = 0;
	//更新
	virtual void Update() = 0;

protected:

	//敵の情報
	IEnemy* enemy_;

};
