#pragma once
#include <Adapter.h>
#include <vector>
#include "../Condition/ICondition.h"

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
	~Combo() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグの
	/// </summary>
	void DebugGUI(const std::string id);

public: // アクセッサ等
	
	/// <summary>
	/// 開始条件の達成状況ゲッター
	/// </summary>
	/// <returns>コンボの開始条件を満たしているか</returns>
	bool GetConditions();

private: // メンバ変数

	// 派生コンボ先配列
	std::vector<Combo> child_;

	// コンボの開始条件配列
	std::vector<LWP::Utility::ICondition> conditions_;

	// 攻撃判定開始秒数
	float attackStartTime_ = 0.0f;
	// 攻撃判定終了秒数
	float attackEndTime_ = 0.0f;

	// 次のコンボへの受付開始時間
	float ReceptTime_ = 0.0f;
};

