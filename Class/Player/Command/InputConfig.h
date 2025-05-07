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
			uint8_t Evasion = DIK_LSHIFT;					// 回避
		};

		// ゲームパッド
		namespace GamePad {
			uint8_t NormalAttack = XBOX_X;					// 通常攻撃
			uint8_t Parry = XBOX_RT;						// パリィ
			uint8_t LockOn = XBOX_LT;						// ロックオン
			uint8_t Evasion = XBOX_LB | XBOX_RB;			// 回避
		};
	};
};