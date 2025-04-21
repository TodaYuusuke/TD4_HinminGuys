#include "LockOn.h"

LockOn::LockOn(LWP::Object::Camera* camera) {
	pCamera_ = camera;
}

void LockOn::Initialize() {
	isActive_ = false;
}

void LockOn::Update() {
	// 入力処理
	InputUpdate();


}

void LockOn::Reset() {
	lockOnDatas_.clear();
}

void LockOn::InputUpdate() {
	// LTでロックオン
	if (LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_LEFT_THUMB)) {
		if (!isActive_) {
			isActive_ = true;
		}
		else {
			isActive_ = false;
			Reset();
		}
	}
}

void LockOn::SearchLockOnEnemy() {
	for (LockOnData& lockOnData : lockOnDatas_) {
		// 一度ロックオンしたらスキップ
		if (lockOnData.isActive) { continue; }

		// カメラの正面方向に敵がいるか

		// ロックオン可能距離に敵がいるかを算出

	}
}

void LockOn::SearchNearEnemy() {
	// ロックオンリスト内の敵で自機に最も近い敵を最初にロックオン

}

void LockOn::ClearLockOnList() {
	// ロックオン範囲外

	// カメラの正面方向にいない
}