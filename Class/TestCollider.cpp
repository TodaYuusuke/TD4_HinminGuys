#include "TestCollider.h"

TestCollider::TestCollider()
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	model_.LoadShortPath("player/Player_Simple.gltf");

	// 攻撃判定生成
	aabb_.min.y = 0.0f;
	aabb_.max.y = 1.0f;
	collider_.isActive = true;/*
	collider_.mask.SetBelongFrag(ColMask1);
	collider_.mask.SetHitFrag((uint32_t)~ColMask1);*/
}

void TestCollider::Initialize()
{
}

void TestCollider::Update()
{
}
