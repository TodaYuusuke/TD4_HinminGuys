#include "PressKeyboardCondition.h"

bool LWP::Utility::PressKeyboardCondition::CheckCondition()
{
	// キーの長押し入力を検知してそれを返す
	return LWP::Input::Keyboard::GetPress(kId_);
}
