#pragma once
#include <Adapter.h>
#include "ICameraSystem.h"

class FovSystem : ICameraSystem {
public:
	FovSystem() = default;
	~FovSystem()override = default;
private:
};

class TargetOffsetSystem : ICameraSystem {
public:
	TargetOffsetSystem() = default;
	~TargetOffsetSystem()override = default;
private:
};