#include "PlayerParameter.h"

void PlayerParameter::Initialize() {
	// 鞘
	sheath_ = {
		10.0f,
		1.0f
	};
	// 攻撃
	attack_ = {
		10.0f,
		1.0f
	};
	// 速度
	speed_ = {
		0.0f,
		1.0f
	};
	// パラメータの名前
	name_ = "Player";
}

void PlayerParameter::Update() {

}
