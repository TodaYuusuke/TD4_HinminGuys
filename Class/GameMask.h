#pragma once
#include "../Adapter/Adapter.h"
#include "../Engine/object/core/collision/Collision.h"

namespace GameMask {
	// 自機
	inline int GetPlayer() { return ColMask0; }
	// 敵
	inline int GetEnemy() { return ColMask1; }
	// 攻撃
	inline int GetAttack() { return ColMask2; }
};