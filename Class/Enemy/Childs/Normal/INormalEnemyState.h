#pragma once
#include "../Adapter/Adapter.h"

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
	kHitReaction,
	kMax,
};

class Normal;

/// <summary>
/// 敵の行動パターン基底クラス
/// </summary>
class INormalEnemyState
{
public:

	virtual ~INormalEnemyState() = default;
	//初期化
	virtual void Initialize() = 0;
	//更新
	virtual void Update() = 0;

	States& GetState() { return stateType_; }

protected:

	//敵の情報
	Normal* enemy_;
	//ステートの保持しているタイプ
	States stateType_;

};
