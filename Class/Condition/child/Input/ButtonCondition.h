#pragma once
#include "../../ICondition.h"
#include "../../../DeltaTimer/DeltaTimer.h"

namespace LWP::Utility {

	/// <summary>
	/// コントローラーボタンの入力による分岐条件
	/// </summary>
	class ButtonCondition : public LWP::Utility::ICondition
	{
	public: // コンストラクタ等

		// デフォルトコンストラクタの削除
		ButtonCondition() = delete;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="inputState">入力状態</param>
		/// <param name="buttonID">ボタンのID</param>
		/// <param name="keyID">キーのID</param>
		ButtonCondition(int inputState, int buttonID, int keyID) :inputState_(inputState), buttonID_(buttonID), keyID_(keyID) {};

	public: // メンバ関数

		/// <summary>
		/// 更新関数
		/// </summary>
		void Update() override;

		/// <summary>
		/// デバッグ用GUIの表示関数
		/// </summary>
		void DebugGUI() override;

	public: // アクセッサ等

		/// <summary>
		/// 分岐条件を達成しているか
		/// </summary>
		/// <returns>分岐条件の達成成否</returns>
		bool CheckCondition() override;

	private: // エディタ用関数
		
		/// <summary>
		/// 入力状態の設定
		/// </summary>
		void InputStateSettings();

		/// <summary>
		/// ボタン関係の設定
		/// </summary>
		void ButtonSettings();

	private: // メンバ変数

		// 入力の条件
		int inputState_{};

		// 長押し入力時の必要秒数
		float pressTime_ = 0.1f;
		// 長押し入力検証用のタイマー
		LWP::Utility::DeltaTimer pressTimer_{};

		// 検証するボタンID
		int buttonID_{};

		// 検証するキーID
		int keyID_{};

	};

}

