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
			int NormalAttack = XBOX_Y;						// 通常攻撃
			int Parry = XBOX_RT;							// パリィ
			int LockOn = XBOX_LT;							// ロックオン
			int Evasion = XBOX_LB | XBOX_RB;				// 回避
		};
	};
};