#pragma once
#include <stdint.h>
#include "../Adapter/Adapter.h"
///
/// それぞれのステートで使用するパラメータをまとめたヘッダー
/// 

namespace OgreState {

	//翁雅のパラメータ調整項目
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
		kIdle, //待機
		kMove, //移動
		kSwingDownAttack, //ふり降ろし
		kRotatingSlash, //回転斬り
		kFallingThrust, //落下攻撃
		kAssaultSlash, //連続突撃
		kQuadrupleAttack, //四連撃
		kHitReaction, //ヒット演出
		kMax, //最大数
	};

	/// <summary>
	/// 攻撃の強さ
	/// </summary>
	enum class AttackStrength {
		kLight,
		kMedium,
		kHeavy,
		kMax,
	};

	/// <summary>
	/// 停止状態パラメータ
	/// </summary>
	struct IdleParameter {
		//待機時間
		static float standTime;
		//追従モーションに移行する距離
		static float followingDist;
		//カウント
		float countStandTime;
	};

	/// <summary>
	/// 移動パラメータ
	/// </summary>
	struct MoveParameter {
		//移動時間
		static float runTime;
		//攻撃モーションに入る距離
		static float attackDist;
		//デフォのスピード
		static float defaultSpeed;
		//弱攻撃からの遷移に必要なカウント
		static int32_t lightTransitionCount;
		//中攻撃からの遷移に必要なカウント
		static int32_t mediumTransitionCount;
		//移動方向
		LWP::Math::Vector3 direction;
		//速度
		float speed;
		//カウント
		float countRunTime;
		//弱攻撃カウント
		int32_t lightAttackCount = 0;
		//中攻撃カウント
		int32_t mediumAttackCount = 0;
		//プレイヤーに近づくかどうか
		bool isApproach = true;
	};

	/// <summary>
	/// 降り降ろし攻撃パラメータ
	/// </summary>
	struct SwingDownAttack {
		//攻撃判定受付開始時間
		static float startAcceptTime;
		//攻撃判定受付終了時間
		static float endAcceptTime;
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kLight;
	};

	/// <summary>
	/// 回転斬りパラメータ
	/// </summary>
	struct RotatingSlash {
		//攻撃判定受付開始時間
		static float startAcceptTime;
		//攻撃判定受付終了時間
		static float endAcceptTime;
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kLight;
	};

	/// <summary>
	/// 落下突きパラメータ
	/// </summary>
	struct FallingThrust {
		//攻撃判定受付開始時間
		static float startAcceptTime;
		//攻撃判定受付終了時間
		static float endAcceptTime;
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kMedium;
	};

	/// <summary>
	/// 連続突撃斬り
	/// </summary>
	struct AssaultSlash {
		//攻撃判定受付開始時間
		static float startAcceptTime;
		//攻撃判定受付終了時間
		static float endAcceptTime;
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kHeavy;
	};

	/// <summary>
	/// 四連攻撃
	/// </summary>
	struct QuadrupleAttack {
		//攻撃判定受付開始時間
		static float startAcceptTime;
		//攻撃判定受付終了時間
		static float endAcceptTime;
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kHeavy;
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
		MoveParameter moveParameter;

	};

}
