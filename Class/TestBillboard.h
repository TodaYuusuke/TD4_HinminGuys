#pragma once
#include <Adapter.h>

class TestBillboard {
public:
	TestBillboard();
	~TestBillboard() = default;

	void Init();
	void Update();

private:
	LWP::Primitive::NormalStretchedBillboard plane_;
	LWP::Primitive::NormalBillboard2D billboard_;

	LWP::Math::Vector3 velocity_;
};