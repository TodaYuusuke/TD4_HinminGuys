#pragma once
#include <Adapter.h>

// 条件分岐に関わる列挙子を管理する

namespace LWP::Utility::Condition {

	// 入力条件
	enum InputState {
		TRIGGER,	// 押した瞬間
		PRESS,		// 長押し
		RELEASE		// 離したとき
	};

	namespace Controller
	{
		// コントローラーのボタンID
		enum ControllerButton {
			A = XINPUT_GAMEPAD_A,
			B = XINPUT_GAMEPAD_B,
			X = XINPUT_GAMEPAD_X,
			Y = XINPUT_GAMEPAD_Y,
			RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
			LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
			DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
			DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
			DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
			DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT
		};
	}

	namespace KeyBoard
	{
		// キーボードの主要ボタンの割り当て
		enum KeyInput {
			W = DIK_W,
			A = DIK_A,
			S = DIK_S,
			D = DIK_D,
			E = DIK_E,
			Q = DIK_Q,
			UP = DIK_UP,
			LEFT = DIK_LEFT,
			DOWN = DIK_DOWN,
			RIGHT = DIK_RIGHT,
			SHIFT = DIK_LSHIFT,
			SPACE = DIK_SPACE
		};
	}
}

