#pragma once
#include <Adapter.h>
#include "ConditionEnums.h"

namespace LWP::Utility {

	/// <summary>
	/// 分岐条件基底クラス
	/// </summary>
	class ICondition
	{
	public: // コンストラクタ等

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		ICondition() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~ICondition() = default;

	public: // メンバ関数

		/// <summary>
		/// 更新関数
		/// </summary>
		virtual void Update() {};

		/// <summary>
		/// デバッグ用GUIの表示関数
		/// </summary>
		virtual void DebugGUI() { };

		/// <summary>
		/// 保存関数
		/// </summary>
		/// <param name="json">保存対象json</param>
		virtual void Save(LWP::Utility::JsonIO& json) { json; };

	public: // アクセッサ等
		
		/// <summary>
		/// 分岐条件を達成しているか
		/// </summary>
		/// <returns>分岐条件の達成成否</returns>
		virtual bool CheckCondition() = 0;

		/// <summary>
		/// 削除フラグ状態ゲッター
		/// </summary>
		/// <returns>削除フラグの状態</returns>
		bool GetIsDelete() { return imGuiIsDelete_; }
		/// <summary>
		/// 削除フラグ状態セッター
		/// </summary>
		/// <param name="isDelete">削除するか</param>
		void SetIsDelete(const bool isDelete) { imGuiIsDelete_ = isDelete; }

	protected: // メンバ変数

		#pragma region エディタ用変数

		// 条件名
		std::string name_ = "";

		// 条件削除フラグ
		bool imGuiIsDelete_ = false;

		#pragma endregion
	};
}

