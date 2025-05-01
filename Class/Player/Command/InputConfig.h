#pragma once
#include "../Adapter/Adapter.h"

/// <summary>
/// ボタンの配置を定義する
/// </summary>
namespace InputConfig {
	// キーボード
	namespace Key {	
		int NormalAttack = DIK_E;							// 通常攻撃
		int Parry		 = DIK_SPACE;						// パリィ
		int LockOn		 = DIK_Q;							// ロックオン
	};
	
	// ゲームパッド
	namespace GamePad {	
		int NormalAttack = XINPUT_GAMEPAD_X;				// 通常攻撃
		int Parry		 = XINPUT_GAMEPAD_RIGHT_SHOULDER;	// パリィ
		int LockOn		 = XINPUT_GAMEPAD_LEFT_THUMB;		// ロックオン
	};
};