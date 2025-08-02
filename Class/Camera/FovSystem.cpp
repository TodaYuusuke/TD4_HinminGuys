#include "FovSystem.h"
#include "../Player/Math/MathFunctions.h"
#include "../Components/HitStopController.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;
using namespace MathFunc;

FovSystem::FovSystem(const float& defaultFov) {
	defaultFov_ = defaultFov;
	// 線形補間関数追加
	easingFunc_[LerpType::LERP] = &LerpF;
	easingFunc_[LerpType::EXPONENTIAL] = &ExponentialInterpolateF;
}

void FovSystem::Init() {
	goalFov_ = 0.0f;
	startFov_ = 0.0f;
	currentFov_ = 0.0f;

	currentFrame_ = 0.0f;
	endFrame_ = 0.0f;
	returnStayFrame_ = 0.0f;

	isActive_ = false;
	isEasing_ = false;
}

void FovSystem::Update() {
	if (!isActive_) { 
		Init();
		return;
	}
	// イージングが終了したらstartFovの値に戻す
	ReturnFov();

	if (!isEasing_) { return; }
	// イージング終了
	if (currentFrame_ <= 0.0f) { 
		isEasing_ = false; 
		return;
	}

	// イージング計算
	currentFov_ = easingFunc_[LerpType::LERP](startFov_, goalFov_, Easing::OutExpo((endFrame_ - currentFrame_) / endFrame_));

	currentFrame_ -= HitStopController::GetInstance()->GetDeltaTime();
}

void FovSystem::ReturnFov() {
	if (isEasing_) { return; }

	// startFovに戻す
	if (returnStayFrame_ <= 0.0f) {
		currentFov_ = easingFunc_[LerpType::EXPONENTIAL](currentFov_, startFov_, 0.05f);
	}

	returnStayFrame_ -= HitStopController::GetInstance()->GetDeltaTime();

	if (currentFov_ <= startFov_ + 0.5f && currentFov_ >= startFov_ - 0.5f) {
		currentFov_ = startFov_;
		isActive_ = false;
	}
}

void FovSystem::Start(const float& currentFov, const float& goalFov, const float& endFrame, const float& returnStayFrame) {
	startFov_ = currentFov;
	goalFov_ = goalFov;

	endFrame_ = endFrame;
	currentFrame_ = endFrame;
	returnStayFrame_ = returnStayFrame;

	isActive_ = true;
	isEasing_ = true;
}