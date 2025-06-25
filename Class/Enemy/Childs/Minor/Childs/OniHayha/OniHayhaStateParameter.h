#pragma once
#include <stdint.h>
#include "../Adapter/Adapter.h"
///
/// それぞれのステートで使用するパラメータをまとめたヘッダー
/// 

namespace OniHayhaState {

	//才二君のパラメータ調整項目
	class ParameterConfig {
	public:

		LWP::Utility::JsonIO json;

		//JSON初期化
		void InitJson();

	};

	/// <summary>
	/// 状態一覧
	/// </summary>
	enum class States {
		kIdle,
		kAttack,
		kRetreat,
		kWaitingForAttack,
		kHitReaction,
		kAiming,
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
		float countStandTime = 1.0f;
	};

	/// <summary>
	/// 攻撃パラメータ
	/// </summary>
	struct AttackParameter {
		//攻撃判定受付開始時間
		static float startAcceptTime;
		//攻撃判定受付終了時間
		static float endAcceptTime;
		//弾の速度
		static float bulletSpeed;
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
	};

	/// <summary>
	/// ヒット演出パラメータ
	/// </summary>
	struct HitReactionParameter {
		//減衰
		static float decay;
	};

	/// <summary>
	/// エイム中のパラメータ
	/// </summary>
	struct AimingParameter {
		//狙う時間
		static float aimingTime;
		//レーザーがチカチカし始める時間
		static float flickeringTime;
		//チカチカ間隔
		static int32_t flickeringInterval;
		//残り時間カウント
		float countAimingTime;
		//チカチカカウント
		int32_t flickeringCounter;
	};

	/// <summary>
	/// 雑魚敵の全てのパラメータ
	/// </summary>
	struct StateParameter {
		IdleParameter idleParameter;
		AimingParameter aimingParameter;
		WaitingForAttackParameter waitingForAttackParameter;

	};

}

