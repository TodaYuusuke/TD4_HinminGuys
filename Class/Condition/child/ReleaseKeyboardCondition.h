#pragma once
#include "../ICondition.h"

namespace LWP::Utility {

	/// <summary>
	/// キーボードのリリース入力による分岐条件
	/// </summary>
	class TriggerKeyboardCondition : public LWP::Utility::ICondition
	{
	public: // コンストラクタ等

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		TriggerKeyboardCondition() = delete;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="id">キーID</param>
		TriggerKeyboardCondition(int id) : kId_(id) {};

	public: // アクセッサ等

		/// <summary>
		/// 分岐条件を達成しているか
		/// </summary>
		/// <returns>分岐条件の達成成否</returns>
		bool CheckCondition() override;

	private: // メンバ変数

		// キーID
		const int kId_;

	};

}

