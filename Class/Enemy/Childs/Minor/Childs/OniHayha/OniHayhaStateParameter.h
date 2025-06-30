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
		kAiming,
		kMax,
	};

	/// <summary>
	/// 停止状態パラメータ
	/// </summary>
	struct IdleParameter {
		//待機時間
		float standTime;
		//退避モーションに移行する距離
		float retreatDist;
		//カウント
		float countStandTime = 1.0f;
	};

	/// <summary>
	/// 攻撃パラメータ
	/// </summary>
	struct AttackParameter {
		//攻撃判定受付開始時間
		float startAcceptTime;
		//攻撃判定受付終了時間
		float endAcceptTime;
		//弾の速度
		float bulletSpeed;
	};

	/// <summary>
	/// 退避パラメータ
	/// </summary>
	struct RetreatParameter {
		//待機状態に戻る距離
		float idleDist;
	};

	/// <summary>
	/// 攻撃待機パラメータ
	/// </summary>
	struct WaitingForAttackParameter {
		//自身の攻撃する順番
		uint16_t attackID;
	};

	/// <summary>
	/// ヒット演出パラメータ
	/// </summary>
	struct HitReactionParameter {
		//減衰
		float decay;
	};

	/// <summary>
	/// エイム中のパラメータ
	/// </summary>
	struct AimingParameter {
		//狙う時間
		float aimingTime;
		//レーザーがチカチカし始める時間
		float flickeringTime;
		//チカチカ間隔
		int32_t flickeringInterval;
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
		AttackParameter attackParameter;
		RetreatParameter retreatParameter;
		WaitingForAttackParameter waitingForAttackParameter;
		HitReactionParameter hitReactionParameter;
		AimingParameter aimingParameter;

	};

	//オニ・ヘイヘのパラメータ調整項目
	class ParameterConfig {
	public:

		LWP::Utility::JsonIO json;

		//JSON初期化
		void InitJson();
		//パラメータ取得
		const StateParameter& GetStateParameter() const { return stateParameter_; }

	private:
		//編集用パラメータ
		StateParameter stateParameter_{};

	};

}

