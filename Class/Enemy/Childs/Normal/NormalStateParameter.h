#pragma once
#include <stdint.h>
#include "../Adapter/Adapter.h"
///
/// それぞれのステートで使用するパラメータをまとめたヘッダー
/// 

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

/// <summary>
/// 停止状態パラメータ
/// </summary>
struct IdleParameter {
	//待機時間
	static int32_t standTime_;
	//追従モーションに移行する距離
	static float followingDist_;
	//カウント
	int32_t countStandTime;
};

/// <summary>
/// 移動パラメータ
/// </summary>
struct MoveParameter {
	//移動時間
	static int32_t runTime_;
	//攻撃モーションに入る距離
	static float attackDist_;
	//移動方向
	LWP::Math::Vector3 velocity;
	//カウント
	int32_t countRunTime;
	//プレイヤーに近づくかどうか
	bool isApproach = true;
};

/// <summary>
/// 攻撃パラメータ
/// </summary>
struct AttackParameter {
	//攻撃判定受付時間
	static float attackAcceptTime_;
};

/// <summary>
/// 間合い取りパラメータ
/// </summary>
struct SpacingParameter {
	//間合いを取る時間
	static int32_t spacingTime_;
	//間合い距離
	static float spaceDist_;
	//カウント
	int32_t countSpacingTime;
	//右回りかどうか
	bool isClockwise = false;
};

/// <summary>
/// 攻撃待機パラメータ
/// </summary>
struct WaitingForAttackParameter {
	//攻撃の順番を決める変数
	static uint16_t attackCount_;
	//次攻撃するのは何番かを決める変数
	static uint16_t nextAttackCount_;
	//自身の攻撃する順番
	uint16_t attackID;
	//右回りかどうか
	bool isClockwise = false;
};

/// <summary>
/// 追従パラメータ
/// </summary>
struct FollowingParameter {
	//待機モーションに移行する距離
	static float idleDist_;
};

/// <summary>
/// ヒット演出パラメータ
/// </summary>
struct HitReactionParameter {
	//減衰
	static float decay_;
};

/// <summary>
/// 雑魚敵の全てのパラメータ
/// </summary>
struct NormalStateParameter {
	IdleParameter idleParameter;
	MoveParameter moveParameter;
	SpacingParameter spacingParameter;
	WaitingForAttackParameter waitingForAttackParameter;

};
