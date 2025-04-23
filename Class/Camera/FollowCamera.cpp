#include "FollowCamera.h"

FollowCamera::FollowCamera(LWP::Object::Camera* camera, LWP::Math::Vector3* targetPos) {
	camera_ = camera;
	targetPos_ = targetPos;
	// 球体を読み込む
	lockOnData_.targetModel.LoadSphere();
	lockOnData_.targetModel.isActive = false;
}

void FollowCamera::Initialize() {
	lockOnData_.targetModel.worldTF.scale = { 2,2,2 };
	lockOnData_.targetTransform = &t;
}

void FollowCamera::Update() {
	// 入力処理
	InputUpdate();

	// ロックオン処理
	LockOnUpdate();

	// カメラの座標を決定
	camera_->worldTF.translation = (*targetPos_) + kTargetDist * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(camera_->worldTF.rotation);

	ImGui::Begin("Camera");
	ImGui::DragFloat3("Translation", &camera_->worldTF.translation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat4("Quaternion", &camera_->worldTF.rotation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat3("Distance", &kTargetDist.x, 0.1f, -100, 100);
	if (lockOnData_.targetTransform) {
		if (ImGui::TreeNode("LockOn")) {
			ImGui::DragFloat3("Translation", &t.translation.x, 0.1f, -100, 100);
			ImGui::DragFloat4("Quaternion", &lockOnData_.targetTransform->rotation.x, 0.1f, -100, 100);
			ImGui::Checkbox("IsLocked", &lockOnData_.isLocked);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void FollowCamera::InputUpdate() {
	// 回転する向き
	LWP::Math::Vector2 dir = { 0.0f,0.0f };

	// キーボードでの回転
	if (LWP::Input::Keyboard::GetPress(DIK_UP)) {
		dir.x += 1.0f;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_DOWN)) {
		dir.x -= 1.0f;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		dir.y += 1.0f;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		dir.y -= 1.0f;
	}

	// コントローラーでの回転
	dir.x -= LWP::Input::Pad::GetRStick().y;
	dir.y += LWP::Input::Pad::GetRStick().x;

	// 角度制限
	ClampAngle(dir.x, ((*targetPos_) - camera_->worldTF.translation).Normalize(), kMinRotateX, kMaxRotateX);

	// x軸回転
	camera_->worldTF.rotation = camera_->worldTF.rotation * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, 0.03f * dir.x);
	// y軸は常に上を向くように固定
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, 0.03f * dir.y) * camera_->worldTF.rotation;
}

void FollowCamera::LockOnUpdate() {
	lockOnData_.targetModel.worldTF.translation = t.translation;

	// ロックオン対象がいないなら早期リターン
	if (!lockOnData_.targetTransform) { return; }
	if (!lockOnData_.isLocked) { return; }

	// ロックオン対象とカメラとの方向ベクトルを算出
	lwp::Vector3 d = (lockOnData_.targetTransform->GetWorldPosition() - camera_->worldTF.translation).Normalize();
	LWP::Math::Vector2 dir;
	dir.y = atan2(d.x, d.z);                        // Y軸（左右）
	dir.x = atan2(-d.y, sqrt(d.x * d.x + d.z * d.z)); // X軸（上下）

	// x軸回転
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, dir.x);
	// y軸は常に上を向くように固定
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, dir.y) * camera_->worldTF.rotation;
}

void FollowCamera::ClampAngle(float& target, LWP::Math::Vector3 distance, float minLimitAngle, float maxLimitAngle) {
	// ターゲットとカメラの角度を求める
	float limitX = std::acos(LWP::Math::Vector3::Dot({ 0,1,0 }, distance));
	// 下
	if (limitX < minLimitAngle && target <= 0.0f) {
		target = 0;
	}
	// 上
	if (limitX > maxLimitAngle && target >= 0.0f) {
		target = 0;
	}
}

LWP::Math::Vector3 FollowCamera::ExponentialInterpolate(const LWP::Math::Vector3& current, const LWP::Math::Vector3& target, float damping) {
	float factor = 1.0f - std::exp(-damping);
	return current + (target - current) * factor;
}
