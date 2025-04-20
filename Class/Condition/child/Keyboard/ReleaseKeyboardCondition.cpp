#include "ReleaseKeyboardCondition.h"

bool LWP::Utility::ReleaseKeyboardCondition::CheckCondition()
{
	// キーのリリース入力を検知してそれを返す
	return LWP::Input::Keyboard::GetRelease(static_cast<uint8_t>(kId_));
}
