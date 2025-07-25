#pragma once
#include <Adapter.h>

class FollowCamera;
class TestBillboard {
public:
	TestBillboard(FollowCamera* followCamera);
	~TestBillboard() = default;

	void Init();
	void Update();

	LWP::Math::Quaternion StrechedBillboad();

public:
	FollowCamera* followCamera_;

private:
	LWP::Primitive::NormalStretchedBillboard plane_;

	LWP::Math::Vector3 velocity_;
};