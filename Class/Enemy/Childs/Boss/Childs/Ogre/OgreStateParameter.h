#pragma once
#include <stdint.h>
#include <array>
#include "../Adapter/Adapter.h"
#include <string>
///
/// それぞれのステートで使用するパラメータをまとめたヘッダー
/// 

namespace OgreState {

	/// <summary>
	/// 状態一覧
	/// </summary>
	enum class States {
		kSpawn, //出現
		kIdle, //待機
		kMove, //移動
		kSwingDownAttack, //ふり降ろし
		kRotatingSlash, //回転斬り
		kFallingThrust, //落下攻撃
		kAssaultSlash, //連続突撃
		kQuadrupleAttack, //四連撃
		kHitReaction, //ヒット演出
		kDead, //死亡
		kSpawnEnemy, //敵追加
		kVoid, //空白ステート(デバッグ用)
		kMax, //最大数
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
		float standTime = 1.0f;
		//追従モーションに移行する距離
		float followingDist = 4.0f;
		//カウント
		float countStandTime = 1.0f;
	};

	/// <summary>
	/// 移動パラメータ
	/// </summary>
	struct MoveParameter {
		//移動時間
		float runTime = 1.0f;
		//攻撃モーションに入る距離
		float attackDist = 1.0f;
		//デフォのスピード
		float defaultSpeed = 3.0f;
		//一定時間を超えたときのスピード
		float boostSpeed = 6.0f;
		//弱攻撃からの遷移に必要なカウント
		int32_t lightTransitionCount = 2;
		//中攻撃からの遷移に必要なカウント
		int32_t mediumTransitionCount = 1;
		//移動方向
		LWP::Math::Vector3 direction{ 0.0f,0.0f,1.0f };
		//速度
		float speed = 1.0f;
		//カウント
		float countRunTime = 0.0f;
		//弱攻撃カウント
		int32_t lightAttackCount = 0;
		//中攻撃カウント
		int32_t mediumAttackCount = 0;
		//プレイヤーに近づくかどうか
		bool isApproach = true;
	};

	/// <summary>
	/// 攻撃に関する詳細データ
	/// </summary>
	struct AttackData {
		//SEのファイルパス
		std::string seFilePath = "";
		//攻撃判定受付開始時間
		float startAcceptTime = 0.05f;
		//攻撃判定受付終了時間
		float endAcceptTime = 0.15f;
		//モーションを終わらせる時間
		float endMotionTime = 0.5f;
		//攻撃発生座標
		LWP::Math::Vector3 attackPosition{};
		//攻撃判定の大きさ
		float attackScale = 1.0f;
		//移動速度
		float moveSpeed = 1.0f;
		//攻撃の間合い
		float attackDistance = 1.0f;
		//SEを鳴らすタイミング
		float sePlayTime = 0.13f;
		//SEがなったかどうかフラグ
		bool isPlayedSE = false;
	};

	/// <summary>
	/// 降り降ろし攻撃パラメータ
	/// </summary>
	struct SwingDownAttack {
		//攻撃発生時間に関するデータ
		AttackData attackData{};
		//移動方向
		LWP::Math::Vector3 attackDirection{};
		//斬撃エフェクトパラメータ
		SlashEffectParam effectParam{};
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kLight;
	};

	/// <summary>
	/// 回転斬りパラメータ
	/// </summary>
	struct RotatingSlash {
		//攻撃発生時間に関するデータ
		AttackData attackData{};
		//斬撃エフェクトパラメータ
		SlashEffectParam effectParam{};
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kLight;
	};

	/// <summary>
	/// 落下突きパラメータ
	/// </summary>
	struct FallingThrust {
		//攻撃発生時間に関するデータ
		AttackData attackData{};
		//ジャンプ前待機時間
		float jumpWaitingTime = 0.2f;
		//ジャンプ時間
		float jumpingTime = 0.3f;
		//ジャンプ後待機時間
		float fallWaitingTime = 0.5f;
		//落下時間
		float fallingTime = 0.3f;
		//落下後攻撃発生時間
		float attackTime = 0.1f;
		//後隙の時間
		float gapTime = 1.0f;
		//現在の時間カウント
		float currentTime = 0.0f;
		//ジャンプの高さ
		float jumpingHeight = 1.0f;
		//イージング開始座標
		LWP::Math::Vector3 startPosition{};
		//イージング終了座標
		LWP::Math::Vector3 endPosition{};
		//ジャンプ先の座標
		LWP::Math::Vector3 jumpingPosition{};
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kMedium;
	};

	/// <summary>
	/// 連続突撃斬り
	/// </summary>
	struct AssaultSlash {
		//攻撃発生時間に関するデータ
		AttackData attackData{};
		//斬撃エフェクトパラメータ
		SlashEffectParam effectParam{};
		//攻撃回数
		int32_t maxAttackCount = 5;
		//現在の攻撃回数
		int32_t currentAttackCount = 0;
		//ダイブ開始地点
		LWP::Math::Vector3 diveStartPosition{};
		//ダイブ終了地点
		LWP::Math::Vector3 diveEndPosition{};
		//攻撃開始地点
		LWP::Math::Vector3 attackStartPosition{};
		//攻撃目標地点
		LWP::Math::Vector3 attackEndPosition{};
		//プレイヤーから離れる量
		float leaveDistance = 5.0f;
		//プレイヤーを切り捨てる時の超過距離
		float overDistance = 2.0f;
		//地面に潜る前の待機時間
		float diveWaitingTime = 1.5f;
		//地面に隠れる時間
		float divingTime = 0.5f;
		//潜る速度
		float diveSpeed = 3.0f;
		//隠れた後の待機時間
		float stealthWaitingTime = 1.0f;
		//攻撃中の待機時間
		float waitingTime = 1.0f;
		//突撃時間
		float assaultTime = 0.5f;
		//後隙の時間
		float gapTime = 3.0f;
		//現在の時間カウント
		float currentTime = 0.0f;
		//剣を刺すときのSE発生時間
		float startSEPlayTime = 0.1f;
		//待機中かどうか
		bool isWaiting = false;
		//突撃中かどうか
		bool isRush = false;
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kHeavy;
	};

	/// <summary>
	/// 四連攻撃
	/// </summary>
	struct QuadrupleAttack {
		//最大攻撃数
		static const int32_t kMaxAttackCount = 4;
		//現在の攻撃回数
		int32_t currentAttackCount = 0;
		//攻撃発生時間に関するデータ
		std::array<AttackData, kMaxAttackCount> multipleAttackData{};
		//斬撃エフェクトパラメータ
		std::array<SlashEffectParam, kMaxAttackCount> effectParam{};
		//次の攻撃モーション名
		std::array<std::string, kMaxAttackCount> nextAttackName{ "Slash", "RushSlash", "Slash", "RotatingSlash" };
		//移動方向
		LWP::Math::Vector3 attackDirection{};
		//攻撃の強さ
		AttackStrength attackStrength = AttackStrength::kHeavy;
	};
	
	/// <summary>
	/// ヒット演出パラメータ
	/// </summary>
	struct HitReactionParameter {
		//減衰
		float decay = 0.95f;
	};

	/// <summary>
	/// 死亡時のパラメータ
	/// </summary>
	struct DeadParameter {
		//後ろに下がる速度
		float stepBackSpeed = -0.5f;
		//後退開始
		float backStartF = 0.15f;
		//後退終了
		float backEndF = 0.25f;
		//後退開始2
		float backStartS = 0.35f;
		//後退終了2
		float backEndS = 0.45f;
	};

	/// <summary>
	/// 敵追加パラメータ
	/// </summary>
	struct SpawnEnemy {
		//現在の追加カウント
		int32_t currentCount = 0;
		//一回目のスポーンライン
		float firstSpawnLine = 0.5f;
		//二回目のスポーンライン
		float secondSpawnLine = 0.25f;
		//SEを鳴らすタイミング
		float sePlayTime = 0.13f;
		//SEがなったかどうかフラグ
		bool isPlayedSE = false;
	};

	/// <summary>
	/// 全てのパラメータ
	/// </summary>
	struct StateParameter {
		SpawnParameter spawnParameter{};
		IdleParameter idleParameter{};
		MoveParameter moveParameter{};
		SwingDownAttack swingDownAttack{};
		RotatingSlash rotatingSlash{};
		FallingThrust fallingThrust{};
		AssaultSlash assaultSlash{};
		QuadrupleAttack quadrupleAttack{};
		HitReactionParameter hitReactionParameter{};
		DeadParameter deadParameter{};
		SpawnEnemy spawnEnemy{};
		//押し出し半径
		float extrusionDist = 1.0f;
	};

	//翁雅のパラメータ調整項目
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
