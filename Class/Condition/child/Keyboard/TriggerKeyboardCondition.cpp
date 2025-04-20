#include "TriggerKeyboardCondition.h"

bool LWP::Utility::TriggerKeyboardCondition::CheckCondition()
{
	// キーのトリガー入力を検知してそれを返す
	return LWP::Input::Keyboard::GetTrigger(static_cast<uint8_t>(kId_));
}
