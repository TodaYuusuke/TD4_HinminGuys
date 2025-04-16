#include "FollowCamera.h"
#include <numbers>

void FollowCamera::Initialize() {
	target_.rotation = { 0,0,0 };
}

void FollowCamera::Update() {
	// 入力処理
	InputUpdate();

	// 上下の回転に制限をかける
	destinationAngle_.x = std::clamp<float>(destinationAngle_.x, kMinAxisX, kMaxAxisX);

	// 最短角度補間
	target_.rotation.y = destinationAngle_.y;//LerpShortAngle(target_.rotation.y, destinationAngle_.y, kRotationSmoothness);
	target_.rotation.x = destinationAngle_.x;//LerpShortAngle(target_.rotation.x, destinationAngle_.x, kRotationSmoothness);

	//target_.translation = *targetPos_;

	//// yaw: Y軸回転（左右）
	//LWP::Math::Quaternion yaw = LWP::Math::Quaternion::CreateFromAxisAngle({ 0, 1, 0 }, target_.rotation.y);

	//// pitch: カメラのローカルX軸回転（上下）
	//LWP::Math::Vector3 right = Rotate(camera_->worldTF.rotation, { 1, 0, 0 }); // カメラのローカルX
	//LWP::Math::Quaternion pitch = LWP::Math::Quaternion::CreateFromAxisAngle(right, target_.rotation.x);

	//// 回転の合成：pitch → yaw の順でかける
	//camera_->worldTF.rotation = (pitch * yaw * camera_->worldTF.rotation).Normalize();




	// 追従座標の補間
	// Z軸方向に distance だけ離れる（ローカルZ-）
	//LWP::Math::Vector3 offset = Rotate(camera_->worldTF.rotation, kTargetDist);
	//camera_->worldTF.translation = (*targetPos_) + offset;
	

	interTarget_ = LWP::Math::Vector3::Slerp(interTarget_, *targetPos_, kFollowRate);
	// オフセットの計算
	LWP::Math::Vector3 offset = CalcOffset();

	// 座標をコピーしてオフセット分ずらす。ただしx座標はずらさない
	camera_->worldTF.translation = interTarget_ + offset;

	camera_->worldTF.rotation = LWP::Math::Quaternion::ConvertDirection((*targetPos_) - camera_->worldTF.translation);

	//// クォータニオンをオイラー角に変換
	//LWP::Math::Vector3 checkAngle = camera_->worldTF.rotation.ConvertQuaternion(target_.rotation);
	////camera_->worldTF.rotation = LWP::Math::Quaternion::ConvertEuler(checkAngle);
	//camera_->worldTF.rotation = QuaternionLookRotation(lwp::Vector3{ 0,0,1 }, ((*targetPos_) - camera_->worldTF.translation).Normalize());

	ImGui::Begin("Camera");
	ImGui::DragFloat2("Rotate", &destinationAngle_.x, 0.1f, -100, 100);
	ImGui::DragFloat3("Distance", &kTargetDist.x, 0.1f, -100, 100);
	ImGui::DragFloat("FollowRate", &kFollowRate, 0.01f, 0, 1);
	ImGui::End();
}

void FollowCamera::InputUpdate() {
	// 入力感度
	const LWP::Math::Vector2 sensitivity = kSensitivity;
#pragma  region キーボード入力
	// Y軸
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		destinationAngle_.y += sensitivity.x;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		destinationAngle_.y -= sensitivity.x;
	}
	// X軸
	if (LWP::Input::Keyboard::GetPress(DIK_UP)) {
		destinationAngle_.x -= sensitivity.y;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_DOWN)) {
		destinationAngle_.x += sensitivity.y;
	}
#pragma endregion

#pragma  region ゲームパッド入力
	// Rスティック
	destinationAngle_.x -= LWP::Input::Controller::GetRStick().y * sensitivity.y;
	destinationAngle_.y += LWP::Input::Controller::GetRStick().x * sensitivity.x;
#pragma endregion
}

float FollowCamera::LerpShortAngle(float a, float b, float t) {
	// 角度差分を求める
	float diff = b - a;

	float pi = 3.14f;

	diff = std::fmod(diff, 2 * pi);
	if (diff < 2 * -pi) {
		diff += 2 * pi;
	}
	else if (diff >= 2 * pi) {
		diff -= 2 * pi;
	}

	diff = std::fmod(diff, 2 * pi);
	if (diff < -pi) {
		diff += 2 * pi;
	}
	else if (diff >= pi) {
		diff -= 2 * pi;
	}

	return a + diff * t;
}

LWP::Math::Vector3 FollowCamera::CalcOffset() const {
	LWP::Math::Vector3 offset = kTargetDist;
	// カメラの角度から回転行列を計算
	LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(target_.rotation);
	// オフセットをカメラの回転に合わせて回転
	offset = offset * rotateMatrix;

	return offset;
}

LWP::Math::Quaternion FollowCamera::QuaternionLookRotation(LWP::Math::Vector3 from, LWP::Math::Vector3 to) {
	LWP::Math::Vector3 f = from.Normalize();
	LWP::Math::Vector3 t = to.Normalize();

	float dot = f.x * t.x + f.y * t.y + f.z * t.z;

	if (dot >= 1.0f) {
		return { 0, 0, 0, 1 }; // 同じ方向：回転なし
	}
	if (dot <= -1.0f) {
		// 真逆の方向 → 90度回転を返す（Y軸かX軸ベース）
		LWP::Math::Vector3 axis = LWP::Math::Vector3::Cross(LWP::Math::Vector3{ 0.0f, 1.0f, 0.0f }, f);
		if (axis.Length() < 0.001f) {
			axis = LWP::Math::Vector3::Cross({ 1.0f, 0.0f, 0.0f }, f);
		}
		axis = axis.Normalize();
		return LWP::Math::Quaternion::CreateFromAxisAngle(axis, 3.14159265f); // 180度回転
	}

	LWP::Math::Vector3 axis = LWP::Math::Vector3::Cross(f, t);
	float s = std::sqrt((1 + dot) * 2.0f);
	float invS = 1.0f / s;

	return {
		axis.x * invS,
		axis.y * invS,
		axis.z * invS,
		s * 0.5f
	};
}

LWP::Math::Vector3 FollowCamera::Rotate(LWP::Math::Quaternion q, const LWP::Math::Vector3& v) const {
	LWP::Math::Quaternion p = { v.x, v.y, v.z, 0.0f };      // ベクトルをクォータニオン化（w=0）
	LWP::Math::Quaternion qConj = q.Conjugate();      // 共役（正規化されていれば逆と同じ）

	LWP::Math::Quaternion result = q * p * qConj;   // q * p * q⁻¹

	return { result.x, result.y, result.z };   // 回転後のベクトルを返す
}
