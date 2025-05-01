#pragma once
#include "../Adapter/Adapter.h"

/// <summary>
/// ボタンの配置を定義する
/// </summary>
namespace InputConfig {
	namespace Command {
		// キーボード
		namespace Key {
			uint8_t NormalAttack = DIK_E;					// 通常攻撃
			uint8_t Parry = DIK_SPACE;						// パリィ
			uint8_t LockOn = DIK_Q;							// ロックオン
		};

		// ゲームパッド
		namespace GamePad {
			uint8_t NormalAttack = XINPUT_GAMEPAD_X;		// 通常攻撃
			uint8_t Parry = XINPUT_GAMEPAD_RIGHT_SHOULDER;	// パリィ
			uint8_t LockOn = XINPUT_GAMEPAD_LEFT_THUMB;		// ロックオン
		};
	};
};