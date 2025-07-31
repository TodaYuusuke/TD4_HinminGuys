#pragma once
#include <stdint.h>
#include "../Adapter/Adapter.h"
///
/// それぞれのステートで使用するパラメータをまとめたヘッダー
/// 

namespace SaijiState {

	/// <summary>
	/// 状態一覧
	/// </summary>
	enum class States {
		kSpawn,
		kIdle,
		kMove,
		kAttack,
		kSpacing,
		kFollowing,
		kWaitingForAttack,
		kHitReaction,
		kDead,
		kMax,
	};

	/// <summary>
	/// 斬撃エフェクト用設定値
	/// </summary>
	struct SlashEffectParam {
		//生成座標
		LWP::Math::Vector3 position{};
		//回転角
		LWP::Math::Vector3 rotate{};
		//スケール
		LWP::Math::Vector3 scale{};
		//表示時間
		float playTime = 1.0f;
		//オフセット
		LWP::Math::Vector3 offset{};
		//色
		LWP::Utility::Color color{};
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
	};

	/// <summary>
	/// 停止状態パラメータ
	/// </summary>
	struct IdleParameter {
		//待機時間
		float standTime = 1.0f;
		//追従モーションに移行する距離
		float followingDist = 6.0f;
		//カウント
		float countStandTime = 1.0f;
	};

	/// <summary>
	/// 移動パラメータ
	/// </summary>
	struct MoveParameter {
		//移動時間
		float runTime = 2.0f;
		//攻撃モーションに入る距離
		float attackDist = 1.0f;
		//デフォのスピード
		float defaultSpeed = 1.0f;
		//移動方向
		LWP::Math::Vector3 direction{};
		//カウント
		float countRunTime = 0.0f;
		//プレイヤーに近づくかどうか
		bool isApproach = true;
	};

	/// <summary>
	/// 攻撃パラメータ
	/// </summary>
	struct AttackParameter {
		//攻撃判定受付開始時間
		float startAcceptTime = 0.05f;
		//攻撃判定受付終了時間
		float endAcceptTime = 0.13f;
		//攻撃発生座標
		LWP::Math::Vector3 attackPosition{0.0f,0.0f,1.0f};
		//攻撃判定の大きさ
		float attackScale = 0.5f;
		//攻撃後の硬直時間
		float freezingTime = 1.5f;
		//現在の硬直時間カウント
		float currentFreezingTime = 0.0f;

	};

	/// <summary>
	/// 間合い取りパラメータ
	/// </summary>
	struct SpacingParameter {
		//間合いを取る時間
		float spacingTime = 1.0f;
		//間合い距離
		float spaceDist = 3.0f;
		//カウント
		float countSpacingTime = 0.0f;
		//右回りかどうか
		bool isClockwise = false;
	};

	/// <summary>
	/// 追従パラメータ
	/// </summary>
	struct FollowingParameter {
		//待機モーションに移行する距離
		float idleDist = 3.0f;
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
	/// ヒット演出パラメータ
	/// </summary>
	struct HitReactionParameter {
		//減衰
		float decay = 0.99f;
	};

	/// <summary>
	/// 雑魚敵の全てのパラメータ
	/// </summary>
	struct StateParameter {
		SpawnParameter spawnParameter;
		IdleParameter idleParameter;
		MoveParameter moveParameter;
		AttackParameter attackParameter;
		SpacingParameter spacingParameter;
		FollowingParameter followingParameter;
		WaitingForAttackParameter waitingForAttackParameter;
		HitReactionParameter hitReactionParameter;
		//押し出し半径
		float extrusionDist = 0.5f;
	};

	//才二君のパラメータ調整項目
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

