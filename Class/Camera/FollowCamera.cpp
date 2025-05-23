#include "FollowCamera.h"

FollowCamera::FollowCamera(LWP::Object::Camera* camera, LWP::Math::Vector3* targetPos) {
	camera_ = camera;
	targetPos_ = targetPos;
}

void FollowCamera::Initialize() {
	json_.Init("FollowCameraData.json");
	json_.AddValue<LWP::Math::Vector3>("TargetDistance", &defaultTargetDist_)
		.AddValue<LWP::Math::Vector3>("GameStartAngle", &kStartAngle)
		.AddValue<float>("MinRotateX", &kMinRotateX)
		.AddValue<float>("MaxRotateX", &kMaxRotateX)
		.AddValue<float>("Sensitivity", &sensitivity)
		.BeginGroup("Rate")
		.AddValue<float>("InterTarget", &interTargetRate)
		.AddValue<float>("TargetDist", &targetDistRate)
		.EndGroup()
		.CheckJsonFile();

	defaultTargetDist_ = kTargetDist;
	lockOnOffset_ = kTargetDist;

	// x軸回転
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, LWP::Utility::DegreeToRadian(kStartAngle.x));
	// y軸は常に上を向くように固定
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, LWP::Utility::DegreeToRadian(kStartAngle.y)) * camera_->worldTF.rotation;
}

void FollowCamera::Update() {
	// 入力処理
	InputUpdate();

	// ロックオン処理
	LockOnUpdate();

	// 座標の補間をしていない座標
	defaultPos_ = (*targetPos_) + kTargetDist * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(camera_->worldTF.rotation);
	// カメラの座標を決定
	interTarget_ = LWP::Utility::Interpolation::Exponential(interTarget_, (*targetPos_), interTargetRate);
	camera_->worldTF.translation = interTarget_ + kTargetDist * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(camera_->worldTF.rotation);
}

void FollowCamera::DebugGUI() {
	if (ImGui::TreeNode("Json")) {
		json_.DebugGUI();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("LockOn")) {
		if (lockOnData_.targetTransform) {
			ImGui::DragFloat3("Translation", &lockOnData_.targetTransform->translation.x, 0.1f, -100, 100);
			ImGui::DragFloat4("Quaternion", &lockOnData_.targetTransform->rotation.x, 0.1f, -100, 100);
			ImGui::DragFloat3("Offset", &lockOnOffset_.x, 0.1f, -100, 100);
			ImGui::Checkbox("IsLocked", &lockOnData_.isLocked);

		}
		ImGui::TreePop();
	}

	ImGui::DragFloat3("Translation", &camera_->worldTF.translation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat4("Quaternion", &camera_->worldTF.rotation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat3("Distance", &kTargetDist.x, 0.1f, -100, 100);
}

void FollowCamera::InputUpdate() {
	// ロックオン対象がいないなら早期リターン
	if (lockOnData_.targetTransform) { return; }
	if (lockOnData_.isLocked) { return; }

	// 回転する向き
	LWP::Math::Vector2 dir = { 0.0f,0.0f };

	// キーボードでの回転
	if (LWP::Input::Keyboard::GetPress(DIK_UP)) {
		dir.x -= sensitivity;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_DOWN)) {
		dir.x += sensitivity;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		dir.y += sensitivity;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		dir.y -= sensitivity;
	}

	// コントローラーでの回転
	dir.x -= LWP::Input::Pad::GetRStick().y * sensitivity;
	dir.y += LWP::Input::Pad::GetRStick().x * sensitivity;

	// 角度制限
	ClampAngle(dir.x, ((*targetPos_) - camera_->worldTF.translation).Normalize(), LWP::Utility::DegreeToRadian(kOriginRotateX + kMinRotateX), LWP::Utility::DegreeToRadian(kOriginRotateX + kMaxRotateX));

	// x軸回転
	camera_->worldTF.rotation = camera_->worldTF.rotation * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, 0.03f * dir.x);
	// y軸は常に上を向くように固定
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, 0.03f * dir.y) * camera_->worldTF.rotation;
}

void FollowCamera::LockOnUpdate() {
	// ロックオン対象がいないなら早期リターン
	if (!lockOnData_.targetTransform) {
		kTargetDist = LWP::Utility::Interpolation::Exponential(kTargetDist, defaultTargetDist_, targetDistRate);
		return;
	}
	if (!lockOnData_.isLocked) {
		kTargetDist = LWP::Utility::Interpolation::Exponential(kTargetDist, defaultTargetDist_, targetDistRate);
		return;
	}

	// ロックオン対象との距離
	float lockOnTargetDist = (lockOnData_.targetTransform->translation - (*targetPos_)).Length();
	lockOnTargetDist = std::clamp<float>(lockOnTargetDist, maxLength / 10.0f, maxLength);

	// ロックオン対象との距離に応じてカメラのオフセットを変更
	// ロックオン対象から遠いほどカメラが上に行く
	lockOnOffset_.y = defaultTargetDist_.y + (2.0f * (lockOnTargetDist / maxLength));
	lockOnOffset_.z = defaultTargetDist_.z - (3.0f * (lockOnTargetDist / maxLength));
	kTargetDist = LWP::Utility::Interpolation::Exponential(kTargetDist, lockOnOffset_, targetDistRate);

	// ロックオン対象とカメラとの方向ベクトルを算出
	LWP::Math::Vector3 cameraPos = camera_->worldTF.translation;
	lwp::Vector3 dist = (lockOnData_.targetTransform->GetWorldPosition() - cameraPos).Normalize();
	LWP::Math::Vector2 dir;
	dir.y = atan2(dist.x, dist.z);                        // Y軸（左右）
	dir.x = atan2(-dist.y, sqrt(dist.x * dist.x + dist.z * dist.z)); // X軸（上下

	// 数値が1なら角度制限を行わない
	float isClampAngle = 1;
	// 角度制限
	ClampAngle(isClampAngle, ((lockOnData_.targetTransform->GetWorldPosition()) - camera_->worldTF.translation).Normalize(), LWP::Utility::DegreeToRadian(kOriginRotateX + 0.0f), LWP::Utility::DegreeToRadian(kOriginRotateX + 80.0f));

	if (isClampAngle == 1) {
		radian_ = { dir.x, dir.y, 0.0f };
		// x軸回転
		camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, dir.x);
	}
	else {
		// x軸回転
		camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian_.x);
	}

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