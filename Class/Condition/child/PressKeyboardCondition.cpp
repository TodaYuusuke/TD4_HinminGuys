#include "PressKeyboardCondition.h"

bool LWP::Utility::PressKeyboardCondition::CheckCondition()
{
	// キーのトリガー入力を検知してそれを返す
	return LWP::Input::Keyboard::GetPress(kId_);
}
