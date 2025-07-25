#include "TestBillboard.h"
#include "Camera/FollowCamera.h"
#include "Player/Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

TestBillboard::TestBillboard(FollowCamera* followCamera) {
    followCamera_ = followCamera;
	plane_.material.texture = Resource::LoadTexture("Effect/Spark.png");
    plane_.material.uvTransform.rotation = { 0.0f, 0.0f, 3.14f / 2.0f };
    //surface_.material.texture = Resource::LoadTexture("Effect/Spark.png");
}

void TestBillboard::Init() {

}

void TestBillboard::Update() {
    plane_.velocity = velocity_;
    //surface_.worldTF.rotation = Quaternion::LookRotation(MathFunc::RotateVector(velocity_, Vector3{ 1,0,0 }, 90.0f));
    ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
    plane_.DebugGUI();
}

LWP::Math::Quaternion TestBillboard::StrechedBillboad() {
    // カメラ方向（Z軸）
    Vector3 zAxis = (followCamera_->GetCamera()->worldTF.GetWorldPosition() - plane_.worldTF.GetWorldPosition()).Normalize();  // カメラ→パーティクル方向（正面）

    // Y軸：速度方向（ストレッチ方向）
    //Vector3 yAxis = velocity_.Normalize();
    Vector3 yAxis = MathFunc::RotateVector(Quaternion::LookRotation(velocity_.Normalize()),Vector3{0,1,0});

    // fallback：速度がゼロなら通常Y軸
    if (Vector3::Dot(velocity_, velocity_) < 1e-6f) {
        yAxis = Vector3(0, 1, 0);
    }

    // X軸：YとZの外積（右方向）
    Vector3 xAxis = (Vector3::Cross(yAxis, zAxis)).Normalize();

    // 再計算：Z軸はXとYから再構成（直交性確保）
    zAxis = Vector3::Cross(xAxis, yAxis);

    // --- Step2: 回転行列を作成 ---
    Matrix4x4 rotMat = {
        xAxis.x, yAxis.x, zAxis.x, 0,
        xAxis.y, yAxis.y, zAxis.y, 0,
        xAxis.z, yAxis.z, zAxis.z, 0,
        0,       0,       0,       1
    };

    Quaternion q = Quaternion::LookRotation(Vector3{ 0,0,1 }*rotMat);
    // --- Step3: クォータニオンに変換 ---
    return q;
}
