#pragma once
#include "Adapter.h"

struct LimitVec3 {
	LWP::Math::Vector3 min;
	LWP::Math::Vector3 max;
};
struct LimitF {
	float min;
	float max;
};

struct ParticleJsonData {
	LimitVec3 velocity;
	LimitVec3 scale;
	LWP::Math::Vector3 firstVel;
	LWP::Math::Vector3 acceleration;
	float multiply;
	float maxElapseTime;	// パーティクルが存在できる時間
	int count;
};