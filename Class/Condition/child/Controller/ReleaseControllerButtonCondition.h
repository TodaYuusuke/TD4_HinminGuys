#pragma once
#include "../../ICondition.h"

namespace LWP::Utility {

	/// <summary>
	/// コントローラーボタンのリリース入力による分岐条件
	/// </summary>
	class ReleaseControllerButtonCondition : public LWP::Utility::ICondition
	{
	public: // コンストラクタ等

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		ReleaseControllerButtonCondition() = delete;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="id">キーID</param>
		ReleaseControllerButtonCondition(int id) : kID_(id) {};

	public: // アクセッサ等

		/// <summary>
		/// 分岐条件を達成しているか
		/// </summary>
		/// <returns>分岐条件の達成成否</returns>
		bool CheckCondition() override;

	private: // メンバ変数

		// キーID
		const int kID_;

	};

}

