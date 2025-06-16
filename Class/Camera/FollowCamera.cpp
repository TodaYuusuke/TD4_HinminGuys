#include "FollowCamera.h"
#include "State/InputCamera.h"
#include "State/ParryCamera.h"
#include "State/LockOnCamera.h"
#include "../Player/Player.h"

FollowCamera::FollowCamera(Player* player, LWP::Object::Camera* camera, LWP::Math::Vector3* targetPos) {
	player_ = player;
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
		.AddValue<float>("Rotate", &rotateRate)
		.EndGroup()
		.BeginGroup("Parry")
		.AddValue<Vector3>("Angle", &parryAngle)
		.AddValue<Vector3>("Distance", &parryDist)
		.AddValue<float>("ZoomTime", &zoomFinishTime)
		.AddValue<float>("HoldTime", &zoomHoldFinishTime)
		.AddValue<float>("ShakeTime", &parryShakeTime)
		.EndGroup()
		.CheckJsonFile();


	kTargetDist = defaultTargetDist_;
	lockOnOffset_ = kTargetDist;

	radian_ = {
		LWP::Utility::DegreeToRadian(kStartAngle.x),
		LWP::Utility::DegreeToRadian(kStartAngle.y)
	};

	// x軸回転
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, LWP::Utility::DegreeToRadian(kStartAngle.x));
	// y軸は常に上を向くように固定
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, LWP::Utility::DegreeToRadian(kStartAngle.y)) * camera_->worldTF.rotation;

	// 状態
	state_ = new InputCamera(this);
}

void FollowCamera::Update() {
	// 状態を確認
	CheckState();

	// 状態
	state_->Update();

	// 座標の補間をしていない座標を算出
	defaultPos_ = (*targetPos_) + kTargetDist * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(camera_->worldTF.rotation);
	// カメラの後追い
	interTarget_ = LWP::Utility::Interpolation::Exponential(interTarget_, (*targetPos_), interTargetRate);
	// カメラの座標を決定
	camera_->worldTF.translation = shakeOffset_ + interTarget_ + kTargetDist * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(camera_->worldTF.rotation);
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
	if (ImGui::Button("Start Parry Camera")) {
		player_->GetSystemManager()->SetIsSuccessParry(true);
	}

	camera_->DebugGUI();

	ImGui::DragFloat3("Translation", &camera_->worldTF.translation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat4("Quaternion", &camera_->worldTF.rotation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat3("Radian", &radian_.x, 0.1f, -1000, 1000);
	ImGui::DragFloat3("Distance", &kTargetDist.x, 0.1f, -100, 100);
}

void FollowCamera::CheckState() {
	// パリィカメラ中は状態を変えさせない
	if (state_->GetStateName() == "Parry") { return; }

	// パリィ成功状態
	if (player_->GetSystemManager()->GetIsSuccessParry()) {
		if (state_->GetStateName() != "Parry") {
			ChangeState(new ParryCamera(player_, this));
			return;
		}
	}
	// 入力受付状態
	if (!lockOnData_.targetTransform && !lockOnData_.isLocked) {
		// カメラと追従対象との距離を初期の値に徐々に戻す
		kTargetDist = LWP::Utility::Interpolation::Exponential(kTargetDist, defaultTargetDist_, targetDistRate);
		if (state_->GetStateName() != "Input") {
			ChangeState(new InputCamera(this));
		}
	}
	// 対象をロックオン状態
	else {
		if (state_->GetStateName() != "LockOn") {
			ChangeState(new LockOnCamera(this));
		}
	}
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

void FollowCamera::ChangeState(IFollowCameraState* pState) {
	delete state_;
	state_ = pState;
}