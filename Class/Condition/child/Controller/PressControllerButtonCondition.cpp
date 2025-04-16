#include "PressControllerButtonCondition.h"

bool LWP::Utility::PressControllerButtonCondition::CheckCondition()
{
	// コントローラーのトリガー入力を検知してそれを返す
	return LWP::Input::Controller::GetPress(kID_);
}
