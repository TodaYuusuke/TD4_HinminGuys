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
		kSpawn,
		kIdle,
		kAttack,
		kRetreat,
		kWaitingForAttack,
		kHitReaction,
		kAiming,
		kDead,
		kMax,
	};

	/// <summary>
	/// 出現パラメータ
	/// </summary>
	struct SpawnParameter {
		//出現時間
		float appearanceTime = 3.0f;
		//現在の時間
		float currentTime = 0.0f;
		//スポーン時の開始Y座標
		float startY = -2.0f;
		//スポーン待機時間
		float spawnWaitingTime = 0.0f;
		//スポーン待機時間カウント
		float currentSpawnWaitingTime = 0.0f;
	};

	/// <summary>
	/// 停止状態パラメータ
	/// </summary>
	struct IdleParameter {
		//待機時間
		float standTime = 2.0f;
		//退避モーションに移行する距離
		float retreatDist = 3.0f;
		//カウント
		float countStandTime = 1.0f;
	};

	/// <summary>
	/// 攻撃パラメータ
	/// </summary>
	struct AttackParameter {
		//攻撃判定受付開始時間
		float startAcceptTime = 0.05f;
		//攻撃判定受付終了時間
		float endAcceptTime = 0.99f;
		//弾の速度
		float bulletSpeed = 50.0f;
		//攻撃後の硬直時間
		float freezingTime = 1.5f;
		//現在の硬直時間カウント
		float currentFreezingTime = 0.0f;
		//SEを鳴らすタイミング
		float sePlayTime = 0.13f;
		//SEがなったかどうかフラグ
		bool isPlayedSE = false;
	};

	/// <summary>
	/// 退避パラメータ
	/// </summary>
	struct RetreatParameter {
		//待機状態に戻る距離
		float idleDist = 5.0f;
	};

	/// <summary>
	/// 攻撃待機パラメータ
	/// </summary>
	struct WaitingForAttackParameter {
		//自身の攻撃する順番
		uint16_t attackID = 0;
	};

	/// <summary>
	/// ヒット演出パラメータ
	/// </summary>
	struct HitReactionParameter {
		//減衰
		float decay = 0.99f;
	};

	/// <summary>
	/// エイム中のパラメータ
	/// </summary>
	struct AimingParameter {
		//狙う時間
		float aimingTime = 1.5f;
		//レーザーがチカチカし始める時間
		float flickeringTime = 1.0f;
		//チカチカ間隔
		int32_t flickeringInterval = 3;
		//残り時間カウント
		float countAimingTime = 0.0f;
		//チカチカカウント
		int32_t flickeringCounter = 0;
		//既に銃を構えているかどうか
		bool isSettingUp = false;
	};

	/// <summary>
	/// 雑魚敵の全てのパラメータ
	/// </summary>
	struct StateParameter {
		SpawnParameter spawnParameter;
		IdleParameter idleParameter;
		AttackParameter attackParameter;
		RetreatParameter retreatParameter;
		WaitingForAttackParameter waitingForAttackParameter;
		HitReactionParameter hitReactionParameter;
		AimingParameter aimingParameter;
		//押し出し半径
		float extrusionDist = 0.5f;
	};

	//オニ・ヘイヘのパラメータ調整項目
	class ParameterConfig {
	public:

		LWP::Utility::JsonIO json;

		//JSON初期化
		void InitJson();
		//パラメータ取得
		StateParameter& GetStateParameter() { return stateParameter_; }

	private:
		//編集用パラメータ
		StateParameter stateParameter_{};

	};

}

