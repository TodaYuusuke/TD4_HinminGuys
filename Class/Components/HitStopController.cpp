#include "HitStopController.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Information;

HitStopController::HitStopController() {

}

HitStopController* HitStopController::GetInstance() {
	static HitStopController instance;
	return &instance;
}

void HitStopController::Initialize() {
	// ヒットストップする時間
	hitStopTime_ = 0.0f;
	// 経過時間
	currentFrame_ = 0.0f;
	// 終了しているかを取得
	isFinish_ = true;
}

void HitStopController::Update() {
	// 数値が入っているならデクリメント
	if (currentFrame_ > 0.0f) {
		currentFrame_--;
	}

	// クールタイムが終了している
	if (currentFrame_ <= 0.0f) {
		// デルタタイムの係数初期化
		if (!isFinish_) {
			SetDeltaTimeMultiply(1.0f);
		}
		isFinish_ = true;

	}
	else {
		isFinish_ = false;
	}
}