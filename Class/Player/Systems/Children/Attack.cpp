#include "Attack.h"
#include "../../Player.h"

using namespace LWP;
using namespace LWP::Object;
using namespace LWP::Object::Collider;

Attack::Attack(LWP::Object::Camera* camera, Player* player)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	player_ = player;

	// 攻撃判定生成
	aabb_.min.y = 0.0f;
	aabb_.max.y = 1.0f;
	collider_.SetFollowTarget(player_->GetWorldTF());
	collider_.isActive = true;
}

void Attack::Initialize() {

}

void Attack::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }
}

void Attack::Reset() {
	isActive_ = true;
}

void Attack::DebugGUI() {
	if (ImGui::TreeNode("Attack")) {
		collider_.DebugGUI();
		ImGui::TreePop();
	}
}