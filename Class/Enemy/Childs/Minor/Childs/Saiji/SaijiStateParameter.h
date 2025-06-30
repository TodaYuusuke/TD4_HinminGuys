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
		kIdle,
		kMove,
		kAttack,
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
		float standTime;
		//追従モーションに移行する距離
		float followingDist;
		//カウント
		float countStandTime = 1.0f;
	};

	/// <summary>
	/// 移動パラメータ
	/// </summary>
	struct MoveParameter {
		//移動時間
		float runTime;
		//攻撃モーションに入る距離
		float attackDist;
		//デフォのスピード
		float defaultSpeed;
		//移動方向
		LWP::Math::Vector3 direction;
		//カウント
		float countRunTime;
		//プレイヤーに近づくかどうか
		bool isApproach = true;
	};

	/// <summary>
	/// 攻撃パラメータ
	/// </summary>
	struct AttackParameter {
		//攻撃判定受付開始時間
		float startAcceptTime;
		//攻撃判定受付終了時間
		float endAcceptTime;
	};

	/// <summary>
	/// 間合い取りパラメータ
	/// </summary>
	struct SpacingParameter {
		//間合いを取る時間
		float spacingTime;
		//間合い距離
		float spaceDist;
		//カウント
		float countSpacingTime;
		//右回りかどうか
		bool isClockwise = false;
	};

	/// <summary>
	/// 追従パラメータ
	/// </summary>
	struct FollowingParameter {
		//待機モーションに移行する距離
		float idleDist;
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
		float decay;
	};

	/// <summary>
	/// 雑魚敵の全てのパラメータ
	/// </summary>
	struct StateParameter {
		IdleParameter idleParameter;
		MoveParameter moveParameter;
		AttackParameter attackParameter;
		SpacingParameter spacingParameter;
		FollowingParameter followingParameter;
		WaitingForAttackParameter waitingForAttackParameter;
		HitReactionParameter hitReactionParameter;

	};

	//才二君のパラメータ調整項目
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

