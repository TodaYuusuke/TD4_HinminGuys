#include "Collect.h"
#include "../../../../Player.h"
#include "../../Sheath.h"
#include "Throw.h"

Collect::Collect(Sheath* sheathSystem, Player* player) {
	sheathSystem_ = sheathSystem;
	player_ = player;

	// 状態の名前
	stateName_ = "Collect";
}

void Collect::Initialize() {

}

void Collect::Update() {
	if (!isActive_) { return; }

	t_++;
	velocity_ = (LWP::Utility::Interpolation::Lerp(start_, end_, LWP::Utility::Easing::OutExpo(t_ / 30.0f)) - player_->GetWorldTF()->GetWorldPosition());

	// 移動速度からラジアンを求める
	radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

	sheathSystem_->SetVelocity(velocity_);
	sheathSystem_->SetRotate(radian_);
	sheathSystem_->SetRotate(quat_);

	if (isActive_ && t_ >= 30.0f) {
		sheathSystem_->Reset();
		sheathSystem_->ChangeState(new Throw(sheathSystem_, player_));
		return;
	}
}

void Collect::Command() {
	isActive_ = true;
	start_ = player_->GetWorldTF()->GetWorldPosition();
	end_ = sheathSystem_->GetSheathWorldTF().GetWorldPosition();
}

void Collect::AnimCommand() {

}

float Collect::SmoothDampF(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
	// Based on Game Programming Gems 4 Chapter 1.10
	float limitTime;
	limitTime = max(0.0001f, smoothTime);
	float omega = 2.0f / limitTime;

	float x = omega * deltaTime;
	float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
	float change = current - target;
	float originalTo = target;

	// Clamp maximum speed
	float maxChange = maxSpeed * limitTime;
	change = std::clamp<float>(change, -maxChange, maxChange);
	float tValue = current - change;

	float temp = (currentVelocity + omega * change) * deltaTime;
	currentVelocity = (currentVelocity - omega * temp) * exp;
	float output = tValue + (change + temp) * exp;

	// Prevent overshooting
	if (originalTo - current > 0.0f == output > originalTo)
	{
		output = originalTo;
		currentVelocity = (output - originalTo) / deltaTime;
	}

	return output;
}
LWP::Math::Vector3 Collect::SmoothDamp(LWP::Math::Vector3 current, LWP::Math::Vector3 target, LWP::Math::Vector3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
	LWP::Math::Vector3 result = {
		SmoothDampF(current.x, target.x, currentVelocity.x,smoothTime, maxSpeed, deltaTime),
		SmoothDampF(current.y, target.y, currentVelocity.y,smoothTime, maxSpeed, deltaTime),
		SmoothDampF(current.z, target.z, currentVelocity.z,smoothTime, maxSpeed, deltaTime)
	};

	return result;
}
