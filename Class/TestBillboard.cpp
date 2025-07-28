#include "TestBillboard.h"
#include "Player/Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

TestBillboard::TestBillboard() {
	plane_.material.texture = Resource::LoadTexture("Effect/Spark.png");
	plane_.material.uvTransform.rotation = { 0.0f, 0.0f, 3.14f / 2.0f };

	billboard_.material.texture = Resource::LoadTexture("Effect/Particle.png");
	billboard_.worldTF.translation = {
		0.0f,
		0.5f,
		0.0f
	};
}

void TestBillboard::Init() {

}

void TestBillboard::Update() {
	plane_.velocity = velocity_;
	//surface_.worldTF.rotation = Quaternion::LookRotation(MathFunc::RotateVector(velocity_, Vector3{ 1,0,0 }, 90.0f));
	ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
	if (ImGui::TreeNode("Stretched")) {
		plane_.DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("2D")) {
		billboard_.DebugGUI();
		ImGui::TreePop();
	}
}