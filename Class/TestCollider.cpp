#include "TestCollider.h"
#include "GameMask.h"

using namespace GameMask;

TestCollider::TestCollider()
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{

}

void TestCollider::Initialize() {
	model_.LoadShortPath("player/Player_Simple.gltf");
	model_.worldTF.translation = { 10,0, 0 };

	// 攻撃判定生成
	aabb_.min.y = 0.0f;
	aabb_.max.y = 1.0f;
	aabb_.SetFollowPtr(&model_.worldTF);
	collider_.isActive = true;
	collider_.mask.SetBelongFrag(GetEnemy() | GetAttack());
	collider_.mask.SetHitFrag(GetPlayer() | GetAttack());
}

void TestCollider::Update()
{
}
