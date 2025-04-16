#include "TriggerKeyboardCondition.h"

bool LWP::Utility::TriggerKeyboardCondition::CheckCondition()
{
	// キーのリリース入力を検知してそれを返す
	return LWP::Input::Keyboard::GetRelease(kId_);
}
