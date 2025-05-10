#pragma once
#include "../Adapter/Adapter.h"

/// <summary>
/// ボタンの配置を定義する
/// </summary>
namespace InputConfig {
	namespace Command {
		// キーボード
		namespace Key {
			inline uint8_t NormalAttack = DIK_E;					// 通常攻撃
			inline uint8_t Parry = DIK_SPACE;						// パリィ
			inline uint8_t LockOn = DIK_Q;							// ロックオン
			inline uint8_t Evasion = DIK_LSHIFT;					// 回避
		};

		// ゲームパッド
		namespace GamePad {
			inline int NormalAttack = XBOX_Y;						// 通常攻撃
			inline int Parry = XBOX_RT;								// パリィ
			inline int LockOn = XBOX_LT;							// ロックオン
			inline int Evasion = XBOX_LB | XBOX_RB;					// 回避
		};
	};
};