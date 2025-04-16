#include "TriggerControllerButtonCondition.h"

bool LWP::Utility::TriggerControllerButtonCondition::CheckCondition()
{
	// コントローラーのトリガー入力を検知してそれを返す
	return LWP::Input::Controller::GetTrigger(kID_);
}
