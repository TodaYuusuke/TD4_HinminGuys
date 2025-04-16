#include "ReleaseControllerButtonCondition.h"

bool LWP::Utility::ReleaseControllerButtonCondition::CheckCondition()
{
	// コントローラーのリリース入力を検知してそれを返す
	return LWP::Input::Controller::GetRelease(kID_);
}
