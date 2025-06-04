#pragma once
#include <stdint.h>
#include "../Adapter/Adapter.h"
///
/// それぞれのステートで使用するパラメータをまとめたヘッダー
/// 

/// <summary>
/// 停止状態パラメータ
/// </summary>
struct IdleParameter {
	//カウント
	int32_t countStandTime;
};

/// <summary>
/// 移動パラメータ
/// </summary>
struct MoveParameter {
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

};

/// <summary>
/// 間合い取りパラメータ
/// </summary>
struct SpacingParameter {
	//カウント
	int32_t countSpacingTime;
	//右回りかどうか
	bool isClockwise = false;
};

/// <summary>
/// 攻撃待機パラメータ
/// </summary>
struct WaitingForAttackParameter {
	//自身の攻撃する順番
	uint16_t attackID;
	//右回りかどうか
	bool isClockwise = false;
};

/// <summary>
/// 追従パラメータ
/// </summary>
struct FollowingParameter {

};

/// <summary>
/// ヒット演出パラメータ
/// </summary>
struct HitReactionParameter {

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
