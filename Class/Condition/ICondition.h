#pragma once
#include <Adapter.h>

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

	public: // アクセッサ等
		
		/// <summary>
		/// 分岐条件を達成しているか
		/// </summary>
		/// <returns>分岐条件の達成成否</returns>
		virtual bool CheckCondition() = 0;

		/// <summary>
		/// 分岐条件名ゲッター
		/// </summary>
		/// <returns>条件分岐名</returns>
		std::string GetName() { return name_; }

	protected: // メンバ変数

		// 分岐条件名
		std::string name_ = "None";

	};
}

