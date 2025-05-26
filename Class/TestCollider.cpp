#include "TestCollider.h"
#include "GameMask.h"

using namespace GameMask;

TestCollider::TestCollider()
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	model_.worldTF.translation = { 0,0, 10 };

	// 攻撃判定生成
	aabb_.min.y = 0.0f;
	aabb_.max.y = 1.0f;
	collider_.isActive = true;
	collider_.mask.SetBelongFrag(GetEnemy());
	collider_.mask.SetHitFrag(GetPlayer() | GetAttack());
}

void TestCollider::Initialize()
{
}

void TestCollider::Update()
{
}
