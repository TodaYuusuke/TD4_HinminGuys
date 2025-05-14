#pragma once
#include <Adapter.h>
#include <list>
#include "../Condition/ConditionList.h"
#include "../DeltaTimer/DeltaTimer.h"

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
	/// <param name="anim">アニメーション</param>
	void Start(LWP::Resource::Animation* anim);

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="model">スキニングモデル</param>
	/// <param name="anim">アニメーション</param>
	void Update(LWP::Resource::SkinningModel* model, LWP::Resource::Animation* anim);

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
	/// 硬直状態ゲッター
	/// </summary>
	/// <returns>硬直状態か</returns>
	bool GetIsStifness() { return isStifness_; }

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
	void AttackActiveUpdate();

	/// <summary>
	/// 攻撃アシストの有効判定関係の更新
	/// </summary>
	void AttackAssistUpdate();

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

	// コンボの名称
	std::string name_ = "";

	// 再生されるアニメーション名
	std::string animName_ = "";

	// 派生コンボ先配列
	std::list<Combo*> childs_;

	// コンボの開始条件配列
	std::list<LWP::Utility::ICondition*> conditions_;

	// このコンボへの派生優先度
	int derivationProiority_ = 0;

	// 攻撃判定開始秒数
	float attackStartTime_ = 0.0f;
	// 攻撃判定終了秒数
	float attackEndTime_ = 0.0f;
	// 攻撃判定処理用タイマー
	LWP::Utility::DeltaTimer attackDecisionTimer_{};
	// 攻撃判定の有効フラグ
	bool isAttackActive_ = false;

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

