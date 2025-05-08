#pragma once
#include "../Adapter/Adapter.h"
#include "../Engine/object/core/collision/Collision.h"

namespace GameMask {
	inline int GetPlayer() { return ColMask0; }
	inline int GetEnemy() { return ColMask1; }
	inline int GetAttack() { return ColMask2; }
};