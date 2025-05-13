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
	void Init(LWP::Resource::SkinningModel* model, LWP::Resource::Animation* anim);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグのImGuiを表示する関数
	/// </summary>
	void DebugGUI();

public: // アクセッサ等



private: // プライベートなメンバ関数

	/// <summary>
	/// 新規作成弥保存を行うメニュー
	/// </summary>
	void FileMenu();

	/// <summary>
	/// ノードエディタメニュー
	/// </summary>
	void NodeMenu();

	/// <summary>
	/// コンボの保存関数
	/// </summary>
	void SaveCombo();
	
	/// <summary>
	/// コンボのロード関数
	/// </summary>
	void LoadCombo();

	/// <summary>
	/// <エディタ用> 同名コンボがいくつあるかのカウンター
	/// </summary>
	/// <param name="name">検証する名称</param>
	/// <returns>同名コンボ数</returns>
	int GetSameNameCount(const std::string& name);

	/// <summary>
	/// アニメーション名を設定をやりやすくするメニュー
	/// </summary>
	void AnimNameEasySetter();

	/// <summary>
	/// <エディタ用> 派生コンボ生成関数
	/// </summary>
	void CreateChildMenu();

	/// <summary>
	/// <エディタ用> 開始条件生成関数
	/// </summary>
	void CreateConditionMenu();

	/// <summary>
	/// <エディタ用> 開始条件の入力に関するメニュー関数
	/// </summary>
	void InputMenu();

	/// <summary>
	/// 削除メニュー
	/// </summary>
	void DeletePopUp();

private: // メンバ変数

	// アニメーションさせるモデル
	LWP::Resource::SkinningModel* animModel_ = nullptr;
	// アニメーション本体
	LWP::Resource::Animation* anim_ = nullptr;

	// コライダー
	LWP::Object::Collision* collider_;

	// 現在のコンボ
	Combo* nowCombo_ = nullptr;
	// 次のコンボ
	Combo* nextCombo_ = nullptr;

#pragma region エディタ用変数

	// jsonIO
	LWP::Utility::JsonIO jsonIO_;

	// 編集モード有効フラグ
	bool enableEditMode_ = false;

	// 選択中のコンボID
	int selectedID_ = 0;

	// 無操作状態のコンボ
	Combo rootCombo_;

	// 編集中のコンボ
	Combo* editingCombo_ = nullptr;

	// 生成する派生コンボ名称
	std::string imGuiChildComboName_;

	// 削除確認用ポップアップフラグ
	bool imGuiIsOpenPopUp_ = false;

#pragma endregion

};

