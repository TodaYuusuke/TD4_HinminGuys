#pragma once
#include <Adapter.h>
#include <list>
#include "../Condition/ICondition.h"
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
	/// <param name="name">コンボ名</param>
	void Init(const std::string& name);

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="fileName">読み込むjsonファイル名</param>
	/// <param name="comboName">読み込むコンボ名</param>
	void Init(const std::string& fileName, const std::string& comboName);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ノードツリー表示用関数
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
	/// 次のコンボへの移行可能状態ゲッター
	/// </summary>
	/// <returns>次のコンボへ移行できるか</returns>
	bool GetIsRecept() { return isRecept_; }

	/// <summary>
	/// 名前ゲッター
	/// </summary>
	/// <returns>名前</returns>
	std::string GetName() { return name_; };

public: // エディタ用関数群

	/// <summary>
	/// 派生コンボ生成関数
	/// </summary>
	/// <param name="name">派生コンボ名</param>
	void CreateChild(const std::string& name);

private: // プライベートなメンバ関数

	/// <summary>
	/// 攻撃判定の有効判定関係の更新
	/// </summary>
	void AttackActiveUpdate();

	/// <summary>
	/// 硬直時間関係の更新
	/// </summary>
	void StifnessTimeUpdate();

	/// <summary>
	/// コンボ受付時間関係の更新
	/// </summary>
	void ReceptTimeUpdate();

	/// <summary>
	/// コンボ受付関数
	/// </summary>
	Combo* ReceptUpdate();

private: // メンバ変数

	// コンボの名称
	std::string name_ = "";

	// 再生されるアニメーション名
	std::string animName_ = "";

	// 派生コンボ先配列
	std::list<Combo*> childs_;

	// コンボの開始条件配列
	std::list<LWP::Utility::ICondition*> conditions_;

	// 攻撃判定開始秒数
	float attackStartTime_ = 0.0f;
	// 攻撃判定終了秒数
	float attackEndTime_ = 0.0f;
	// 攻撃判定処理用タイマー
	LWP::Utility::DeltaTimer attackDecisionTimer_{};
	// 攻撃判定の有効フラグ
	bool isAttackActive_ = false;

	// 硬直時間
	float stifnessTime_ = 0.0f;
	// 硬直時間タイマー
	LWP::Utility::DeltaTimer stifnessTimer_{};
	// 硬直フラグ
	bool isStifness_ = false;

	// 次のコンボへの受付開始秒数
	float receptTime_ = 0.0f;
	// 受付時間用タイマー
	LWP::Utility::DeltaTimer receptTimer_{};
	// 受付可能フラグ
	bool isRecept_ = false;

	#pragma region エディタ用変数

	// ImGui上で選択されているフラグ
	bool imGuiSelected_ = false;

	// コンボ名称
	char imGuiName_[64] = "";
	// 再生されるアニメーション名称
	char imGuiAnimName_[64] = "";

	#pragma endregion

};

