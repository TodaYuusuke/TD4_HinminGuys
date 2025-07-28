#include "ChainNode.h"
#include "../../../../../Components/HitStopController.h"

using namespace LWP;
using namespace LWP::Math;

ChainNode::ChainNode(const int& springConectPoint1, const int& springConectPoint2) {
	spring_.p1 = springConectPoint1;
	spring_.p2 = springConectPoint2;
}

ChainNode::ChainNode(const Spring& spring) {
	spring_ = spring;
}

void ChainNode::Initialize() {

}

void ChainNode::Update() {
	// 質点の値を直接書き換える
	PointMass& p0 = (*pointMassList_)[spring_.p1];
	PointMass& p1 = (*pointMassList_)[spring_.p2];

	// 速度算出
	if (!p1.isFixed) {
		p1.velocity += CalcPointMassVel(p0, p1);
		p1.model.worldTF.translation += p1.velocity;
	}
}

void ChainNode::InverseUpdate() {
	// 質点の値を直接書き換える
	PointMass& p0 = (*pointMassList_)[spring_.p2];
	PointMass& p1 = (*pointMassList_)[spring_.p1];

	// 速度算出
	if (!p1.isFixed) {
		p1.velocity += CalcPointMassVel(p0, p1);
		p1.model.worldTF.translation += p1.velocity;
	}

	Vector3 dir = Vector3{ 0,0,1 } * Matrix4x4::CreateRotateXYZMatrix(p1.model.worldTF.rotation);
	Vector3 c2c = (p0.model.worldTF.GetWorldPosition() - p1.model.worldTF.GetWorldPosition()).Normalize();
	p1.model.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, p1.euler.x) * Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, p1.euler.y);
	p1.model.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, p1.euler.z) * p1.model.worldTF.rotation;
	p1.model.worldTF.rotation = Quaternion::DirectionToDirection(Vector3{0,0,1}, c2c) * p1.model.worldTF.rotation;
}

void ChainNode::CreateJsonData() {

}

void ChainNode::DebugGui() {

}

LWP::Math::Vector3 ChainNode::CalcPointMassVel(PointMass p0, PointMass p1) {
	Vector3 delta = p1.model.worldTF.translation - p0.model.worldTF.translation;
	float dist = delta.Length();
	Vector3 dir = { 0, 0, 0 };
	if (dist > 0.000001f) {  // 0除算防止
		dir = delta / dist;
	}
	else {
		// 離れていないので、ばね力0扱い（または前の値を使う）
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	// フックの法則（ばね力）
	float springForceMag = (dist - spring_.restLength) * -spring_.stiffness;

	// 質点に加わる力のベクトル
	Vector3 springForce = springForceMag * dir;

#pragma region 減衰を含んだ計算
	// 相対速度による減衰
	Vector3 relVel = p1.velocity - p0.velocity;
	// 減衰を含んだ力
	float dampingMag = spring_.damping * Vector3::Dot(relVel, dir);
	Vector3 dampingForce = dampingMag * dir;
#pragma endregion

	Vector3 totalForce = springForce - dampingForce;
	Vector3 acceleration = totalForce / p1.mass;

	// 重力を加える
	acceleration += p1.acceleration;

	return acceleration * HitStopController::GetInstance()->GetDeltaTime();
}

LWP::Math::Vector3 ChainNode::CalcPointMassVel(PointMass p0, PointMass p1, LWP::Math::Vector3 p1Pos) {
	Vector3 delta = p1Pos - p0.model.worldTF.translation;
	float dist = delta.Length();
	Vector3 dir = { 0, 0, 0 };
	if (dist > 0.000001f) {  // 0除算防止
		dir = delta / dist;
	}
	else {
		// 離れていないので、ばね力0扱い（または前の値を使う）
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	// フックの法則（ばね力）
	float springForceMag = (dist - spring_.restLength) * -spring_.stiffness;

	// 質点に加わる力のベクトル
	Vector3 springForce = springForceMag * dir;

#pragma region 減衰を含んだ計算
	// 相対速度による減衰
	Vector3 relVel = p1.velocity - p0.velocity;
	// 減衰を含んだ力
	float dampingMag = spring_.damping * Vector3::Dot(relVel, dir);
	Vector3 dampingForce = dampingMag * dir;
#pragma endregion

	Vector3 totalForce = springForce - dampingForce;
	Vector3 acceleration = totalForce / p1.mass;

	// 重力を加える
	acceleration += p1.acceleration;

	return acceleration * HitStopController::GetInstance()->GetDeltaTime();
}

void ChainNode::SetJsonData(LWP::Utility::JsonIO& json) {
	// ばね
	json.BeginGroup("Spring");
	// 自然長
	json.AddValue("RestLength", &spring_.restLength);
	// ばね定数
	json.AddValue("Stiffness", &spring_.stiffness);
	// 減衰
	json.AddValue("Damping", &spring_.damping);
	json.EndGroup();
}
