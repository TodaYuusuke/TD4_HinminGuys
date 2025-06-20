#pragma once
#include <stdint.h>
#include "../Adapter/Adapter.h"
///
/// それぞれのステートで使用するパラメータをまとめたヘッダー
/// 

namespace OniHayhaState {

	/// <summary>
	/// 状態一覧
	/// </summary>
	enum class States {
		kIdle,
		kAttack,
		kRetreat,
		kWaitingForAttack,
		kHitReaction,
		kMax,
	};

	/// <summary>
	/// 停止状態パラメータ
	/// </summary>
	struct IdleParameter {
		//待機時間
		static float standTime;
		//退避モーションに移行する距離
		static float retreatDist;
		//カウント
		float countStandTime;
	};

	/// <summary>
	/// 攻撃パラメータ
	/// </summary>
	struct AttackParameter {
		//攻撃判定受付開始時間
		static float startAcceptTime;
		//攻撃判定受付終了時間
		static float endAcceptTime;
	};

	/// <summary>
	/// 退避パラメータ
	/// </summary>
	struct RetreatParameter {
		//待機状態に戻る距離
		static float idleDist;
	};

	/// <summary>
	/// 攻撃待機パラメータ
	/// </summary>
	struct WaitingForAttackParameter {
		//攻撃の順番を決める変数
		static uint16_t attackCount;
		//次攻撃するのは何番かを決める変数
		static uint16_t nextAttackCount;
		//自身の攻撃する順番
		uint16_t attackID;
		//右回りかどうか
		bool isClockwise = false;
	};

	/// <summary>
	/// ヒット演出パラメータ
	/// </summary>
	struct HitReactionParameter {
		//減衰
		static float decay;
	};

	/// <summary>
	/// 雑魚敵の全てのパラメータ
	/// </summary>
	struct StateParameter {
		IdleParameter idleParameter;
		WaitingForAttackParameter waitingForAttackParameter;

	};

}

