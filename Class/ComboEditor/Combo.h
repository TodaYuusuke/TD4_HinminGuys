#pragma once
#include <Adapter.h>
#include <list>
#include "../Condition/ConditionList.h"
#include "../Timer/DeltaTimer.h"
#include "../Particles/SlashEffect/SlashEffector.h"
#include "../Audio/SEPlayer.h"

/// <summary>
/// コンボクラス
/// </summary>
class Combo
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Combo() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Combo();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">コンボ名</param>
	void Init(const std::string& name);

	/// <summary>
	/// 開始関数
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="anim">アニメーション</param>
	/// <param name="collider">コライダー</param>
	/// <param name="sePlayer">効果音プレイヤー</param>
	/// <param name="effector">斬撃エフェクター</param>
	void Start(LWP::Resource::SkinningModel* model, LWP::Resource::Animation* anim, LWP::Object::Collision* collider, SEPlayer* sePlayer, SlashEffector* effector);

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="model">スキニングモデル</param>
	/// <param name="anim">アニメーション</param>
	/// <param name="collider">コライダー</param>
	/// <param name="shape">使用するコライダー形状</param>
	void Update(LWP::Resource::SkinningModel* model, LWP::Resource::Animation* anim, LWP::Object::Collision* collider, LWP::Object::Collider::Sphere* shape);

	/// <summary>
	/// コンボ受付関数
	/// </summary>
	Combo* ReceptUpdate();

	/// <summary>
	/// <エディタ用>ノードツリー表示用関数
	/// </summary>
	/// <param name="id">ラジオボタン判定用ID</param>
	/// <param name="buttonID">ボタンID</param>
	/// <param name="combo">変更する編集対象のコンボ</param>
	void NodeMenu(int& id, int& buttonID, Combo*& combo);

	/// <summary>
	/// デバッグのImGuiを表示する関数
	/// </summary>
	void DebugGUI();

	/// <summary>
	/// 硬直タイマーGUI
	/// </summary>
	void StifnesTimerGUI() { stifnessTimer_.DebugGUI("StiffnessTimer"); }

	/// <summary>
	/// 受付時間タイマーGUI
	/// </summary>
	void ReceptTimerGUI() { receptTimer_.DebugGUI("ReceptTimer_"); }

public: // アクセッサ等

	/// <summary>
	/// 無操作状態のコンボであるかのセッター
	/// </summary>
	/// <param name="isRoot">状態</param>
	void SetIsRoot(const bool isRoot) { isRoot_ = isRoot; }
	/// <summary>
	/// 無操作状態のコンボであるかのゲッター
	/// </summary>
	/// <returns>状態</returns>
	bool GetIsRoot() { return isRoot_; }

	/// <summary>
	/// 開始条件の達成状況ゲッター
	/// </summary>
	/// <returns>コンボの開始条件を満たしているか</returns>
	bool GetConditions();

	/// <summary>
	/// 攻撃判定の有効状態ゲッター
	/// </summary>
	/// <returns>攻撃判定の有効状態</returns>
	bool GetIsAttackActivate() { return isAttackActive_; }

	/// <summary>
	/// 攻撃のアシスト判定の有効状態ゲッター
	/// </summary>
	/// <returns>攻撃アシスト判定の有効状態</returns>
	bool GetIsAttackAssistActive() { return isAttackAssistActive_; }

	/// <summary>
	/// 攻撃アシストの移動量ゲッター
	/// </summary>
	/// <returns>攻撃アシスト時の移動量</returns>
	LWP::Math::Vector3 GetAttackAssistMoveAmount() { return attackAssistMoveAmount_; }

	/// <summary>
	/// ダメージ量ゲッター
	/// </summary>
	/// <returns>ダメージ量</returns>
	float GetDamage() { return damage_; }
	
	/// <summary>
	/// ヒットストップ秒数ゲッター
	/// </summary>
	/// <returns>ヒットストップ秒数</returns>
	float GetHitStopTime() { return hitStopTime_; }

	/// <summary>
	/// 攻撃命中時のクールタイムゲッター
	/// </summary>
	/// <returns>命中時のクールタイム</returns>
	float GetHitCoolTime() { return hitCoolTime_; }

	/// <summary>
	/// ノックバック強さゲッター
	/// </summary>
	/// <returns>ノックバック強さ</returns>
	float GetNockBackStrength() { return nockbackStrength_; }

	/// <summary>
	/// 鞘の耐久値減少量ゲッター
	/// </summary>
	/// <returns>鞘の耐久値減少量</returns>
	float GetSheathDurabityLoss() { return sheathDurabityLoss_; }

	/// <summary>
	/// 硬直状態ゲッター
	/// </summary>
	/// <returns>硬直状態か</returns>
	bool GetIsStifness() { return isStifness_; }

	/// <summary>
	/// 硬直状態の進行度ゲッター
	/// </summary>
	/// <returns>硬直状態進行度</returns>
	float GetStifnessProgress() { return stifnessTimer_.GetProgress(); }

	/// <summary>
	/// 硬直秒数ゲッター
	/// </summary>
	/// <returns>硬直秒数</returns>
	float GetStifnessTime() { return stifnessTime_ - (stifnessTime_ * stifnessTimer_.GetProgress()); }

	/// <summary>
	/// 次のコンボへの移行可能状態ゲッター
	/// </summary>
	/// <returns>次のコンボへ移行できるか</returns>
	bool GetIsRecept() { return isRecept_; }

	/// <summary>
	/// 当コンボ終了時、自身に遷移するかどうかのゲッター
	/// </summary>
	/// <returns>自身に遷移するか</returns>
	bool GetIsReturnSelf() { return isReturnSelf_; }

	/// <summary>
	/// 名前ゲッター
	/// </summary>
	/// <returns>名前</returns>
	std::string GetName() { return name_; }

	/// <summary>
	/// アニメーション名ゲッター
	/// </summary>
	/// <returns>アニメーション名</returns>
	std::string GetAnimName() { return animName_; }
	/// <summary>
	/// アニメーション名のセッター
	/// </summary>
	/// <param name="name">設定するアニメーション名</param>
	void SetAnimName(const std::string& name) { animName_ = name; }

	/// <summary>
	/// 同名コンボ数のゲッター
	/// </summary>
	/// <param name="name">検証する名称</param>
	/// <param name="count">カウント用変数の参照</param>
	void SameNameCount(const std::string& name, int& count);

public: // エディタ用関数群

	/// <summary>
	/// コンボの保存関数
	/// </summary>
	/// <param name="json"></param>
	void AddValue(LWP::Utility::JsonIO& json);

	/// <summary>
	/// <エディタ用> 派生コンボ生成関数
	/// </summary>
	/// <param name="name">派生コンボ名</param>
	Combo& CreateChild(const std::string& name);
	
	/// <summary>
	/// <エディタ用>派生優先度
	/// </summary>
	/// <returns>派生優先度</returns>
	int GetDerivationPriority() const { return derivationProiority_; }

	/// <summary>
	/// <エディタ用> 派生優先度によって派生コンボ配列を並び変える
	/// </summary>
	void SortByPriority();

	/// <summary>
	/// <エディタ用> 派生優先度によって全ての派生コンボ配列を再帰的に並び変える
	/// </summary>
	void SortByPriorityAll();

	/// <summary>
	/// <エディタ用> 削除フラグゲッター
	/// </summary>
	/// <returns>削除するか</returns>
	bool GetIsDelete() { return imGuiIsDelete_; }

	/// <summary>
	/// <エディタ用> 派生コンボ内の要素を全て削除する
	/// </summary>
	void DeleteThis();

	/// <summary>
	/// <エディタ用>新規開始条件追加関数
	/// </summary>
	/// <param name="condition">追加する開始条件</param>
	void AddCondition(LWP::Utility::ICondition* condition);

private: // プライベートなメンバ関数

	/// <summary>
	/// 攻撃判定の有効判定関係の更新
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="collider">コライダー</param>
	/// <param name="shape">使用するコライダー形状</param>
	void AttackActiveUpdate(LWP::Resource::SkinningModel* model, LWP::Object::Collision* collider, LWP::Object::Collider::Sphere* shape);

	/// <summary>
	/// 攻撃アシストの有効判定関係の更新
	/// </summary>
	void AttackAssistUpdate();

	/// <summary>
	/// 斬撃エフェクト関連の更新
	/// </summary>
	/// <param name="model">モデル</param>
	void SlashEffectUpdate(LWP::Resource::SkinningModel* model);

	/// <summary>
	/// 効果音関連の更新関数
	/// </summary>
	void SEUpdate();

	/// <summary>
	/// 硬直時間関係の更新
	/// </summary>
	void StifnessTimeUpdate();

	/// <summary>
	/// コンボ受付時間関係の更新
	/// </summary>
	void ReceptTimeUpdate();

	/// <summary>
	/// <エディタ用>削除フラグがたっているものを削除する関数
	/// </summary>
	void DeleteFunc(Combo*& combo);

	/// <summary>
	/// <エディタ用>開始条件の設定
	/// </summary>
	void StartConditionSettings();

	/// <summary>
	/// <エディタ用>派生優先度設定
	/// </summary>
	void PrioritySettings();

	/// <summary>
	/// <エディタ用>アニメーション関連の設定
	/// </summary>
	void AnimSettings();

	/// <summary>
	/// <エディタ用>攻撃判定関連の設定
	/// </summary>
	void AttackSettings();

	/// <summary>
	/// <エディタ用>硬直関連の設定
	/// </summary>
	void StifnessSetiings();

	/// <summary>
	/// <エディタ用>コンボ受付関連の設定
	/// </summary>
	void ReceptSettings();

private: // メンバ変数

	// 効果音再生クラス
	SEPlayer* sePlayer_ = nullptr;

	// コンボの名称
	std::string name_ = "";

	// 再生されるアニメーション名
	std::string animName_ = "";
	// 遷移秒数
	float transitionTime_ = 0.0f;
	// アニメーションの再生速度
	float animSpeed_ = 1.0f;
	// ループフラグ
	bool isLoop_ = false;

	// 派生コンボ先配列
	std::list<Combo*> childs_;

	// コンボの開始条件配列
	std::list<LWP::Utility::ICondition*> conditions_;

	// このコンボへの派生優先度
	int derivationProiority_ = 0;

	// 攻撃判定開始秒数
	float attackStartTime_ = 0.0f;
	// 攻撃判定有効秒数
	float attackEnableTime_ = 0.0f;
	// 攻撃判定処理用タイマー
	LWP::Utility::DeltaTimer attackDecisionTimer_{};
	// 攻撃判定の有効フラグ
	bool isAttackActive_ = false;
	// 攻撃判定の追従先ジョイント名
	std::string followJointName_{};
	// 攻撃判定のオフセット
	LWP::Math::Vector3 attackColliderOffset_{};
	// 攻撃判定サイズ
	float attackColliderRadius_ = 1.0f;

	// 攻撃のダメージ量
	float damage_ = 1.0f;
	// 攻撃のヒットストップ秒数
	float hitStopTime_ = 0.1f;
	// 命中時の攻撃のヒットクールタイム
	float hitCoolTime_ = 0.5f;
	// ノックバック強さ
	float nockbackStrength_ = 0.1f;
	// 命中時の鞘の耐久値減少量
	float sheathDurabityLoss_ = 10.0f;

	// 攻撃アシストの開始秒数
	float attackAssistStartTime_ = 0.0f;
	// 攻撃アシストの有効秒数
	float attackAssistEnableTime_ = 0.0f;
	// アシスト時の移動量
	LWP::Math::Vector3 attackAssistMoveAmount_{};
	// 攻撃アシスト処理用タイマー
	LWP::Utility::DeltaTimer attackAssistTimer_{};
	// 攻撃アシストの有効フラグ
	bool isAttackAssistActive_ = false;

	// 斬撃エフェクタのポインタ
	SlashEffector* slashEffector_ = nullptr;
	// 斬撃エフェクトの開始秒数
	float startSlashEffectTime_ = 0.0f;
	// 斬撃エフェクト用のタイマー
	LWP::Utility::DeltaTimer slashEffectTimer_{};
	// 斬撃エフェクトのオフセット
	LWP::Math::Vector3 slashEffectOffset_{ 0.0f, 0.5f, 0.0f };
	// 斬撃エフェクトの回転角
	LWP::Math::Vector3 slashEffectRotate_ = { 0.0f, 0.0f, 0.0f };
	// 斬撃エフェクトのスケール
	LWP::Math::Vector3 slashEffectScale_ = { 0.5f, 0.5f, 0.5f };
	// 斬撃エフェクト再生秒数
	float playSlashEffectTime_ = 0.0f;

	// 攻撃効果音までのパス
	std::string audioPath_ = "";
	// 再生音量
	float seVolume_ = 1.0f;
	// 攻撃効果音用タイマー
	LWP::Utility::DeltaTimer seTimer_{};
	// 攻撃効果音再生までの秒数
	float playSETime_ = 0.0f;

	// 硬直時間
	float stifnessTime_ = 0.0f;
	// 硬直時間タイマー
	LWP::Utility::DeltaTimer stifnessTimer_{};
	// 硬直フラグ
	bool isStifness_ = false;

	// 次のコンボへの受付開始秒数
	float receptTime_ = 0.5f;
	// 受付時間用タイマー
	LWP::Utility::DeltaTimer receptTimer_{};
	// 受付可能フラグ
	bool isRecept_ = false;

	// このコンボ終了した時、派生先がない場合自分に返ってくるか
	bool isReturnSelf_ = false;

	#pragma region エディタ用変数

	// 無操作状態のコンボか
	bool isRoot_ = false;

	// ImGui上で選択されているフラグ
	bool imGuiSelected_ = false;

	// このコンボの削除フラグ
	bool imGuiIsDelete_ = false;

	#pragma endregion

};

