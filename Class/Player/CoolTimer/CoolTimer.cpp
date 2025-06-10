#include "CoolTimer.h"

CoolTimer::CoolTimer() {
	coolTimes_[CoolTimeTarget::kParry] = {
		0.0f,
		true
	};
	coolTimes_[CoolTimeTarget::kEvasion] = {
		0.0f,
		true
	};
	coolTimes_[CoolTimeTarget::kSheath] = {
		0.0f,
		true
	};
}

void CoolTimer::Initialize() {

}

void CoolTimer::Update() {
	for (std::map<CoolTimeTarget, CoolTimeData>::iterator it = coolTimes_.begin(); it != coolTimes_.end(); ++it) {
		// 数値が入っているならデクリメント
		if (it->second.coolTime > 0.0f) {
			it->second.coolTime--;
		}

		// クールタイムが終了している
		if (it->second.coolTime <= 0.0f) {
			it->second.isFinish = true;
		}
		else {
			it->second.isFinish = false;
		}
	}
}
