#pragma once
#include <Adapter.h>
#include <list>
#include "Combo.h"

/// <summary>
/// コンボツリークラス
/// </summary>
class ComboTree
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ComboTree() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ComboTree();

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
	/// デバッグのImGuiを表示する関数
	/// </summary>
	void DebugGUI();

public: // 機能関数群

	/// <summary>
	/// <エディタ用> 派生コンボ追加関数
	/// </summary>
	void CreateChild();

public: // アクセッサ等



private: // プライベートなメンバ関数



private: // メンバ変数

	// 現在のコンボ
	Combo* nowCombo_ = nullptr;
	// 次のコンボ
	Combo* nextCombo_ = nullptr;

#pragma region エディタ用変数

	// 選択中のコンボID
	int selectedID_ = 0;

	// 大元コンボ配列
	std::list<Combo*> baseCombos_;

	// 編集中のコンボ
	Combo* editingCombo_ = nullptr;

#pragma endregion

};

