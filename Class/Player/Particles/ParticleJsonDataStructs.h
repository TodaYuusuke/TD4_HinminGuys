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

// 各パーティクルの情報
struct ParticleData {
	//LWP::Primitive::NormalStretchedBillboard billboard;
	//LWP::Resource::RigidModel plane;		// 平面
	LWP::Math::Vector3 vel;					// 速度
	LWP::Math::Vector3 euler;				// オイラー角
	float multiply;
	float lifeTime;							// 生存時間
	float currentTime = 0;					// 経過フレーム
	int type;
	bool isAlive = true;
	//std::function<void(ParticleData&, LWP::Primitive::IPlane&)> updateFunc;
};
// Particleを発生させる
struct Emitter {
	LWP::Math::Vector3 pos;
	int32_t count;
	int32_t spawnCount;
	int32_t spawnLeft = 1;	// 発生の残り回数 
	float frequency;
	float frequencyTime;
};