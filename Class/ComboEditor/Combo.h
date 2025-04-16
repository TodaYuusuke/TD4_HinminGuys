#pragma once
#include <Adapter.h>
#include <vector>

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

public: // アクセッサ等
	
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetConditions();

private: // メンバ変数

	// 派生コンボ先
	std::vector<Combo> child_;

	// 開始条件
	

};

