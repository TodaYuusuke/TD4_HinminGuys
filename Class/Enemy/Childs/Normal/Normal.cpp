#include "Normal.h"
#include "../../../Player/Player.h"
#include "../../DirectXGame/Engine/primitive/model/Material.h"
#include "../../../GameMask.h"

using namespace LWP::Primitive;
using namespace GameMask;

Normal::~Normal()
{

	

}

void Normal::Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
	EnemyManager* manager)
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	type_ = EnemyType::kNormal;
	//アニメーションロード
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	swordModel_.LoadShortPath("player/SimpleWeapon.gltf");
	model_.materials["Material"].color = { 1.0f,0.0f,0.0f,1.0f };
	SetPlayer(player);
	camera_ = camera;
	enemyManager_ = manager;
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
	//関数セット
	AddStateFunc();
	state_.request = States::kNormalIdle;

	// 刀モデルをプレイヤーの手に追従させる
	swordModel_.GetJoint("Grip")->localTF.Parent(&model_, "WeaponAnchor");
	// 体の判定生成
	collider_.SetFollow(&model_.worldTF);
	collider_.isActive = true;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	// 自機の所属しているマスクを設定
	collider_.mask.SetBelongFrag(GetEnemy());
	// 当たり判定をとる対象のマスクを設定
	collider_.mask.SetHitFrag(GetAttack());
	collider_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;

		//ステートをセット(攻撃中はリアクションしない)
		if (state_.GetCurrentBehavior() != States::kNormalAttack) {
			SetPreState(state_.GetCurrentBehavior());
			state_.request = States::kHitReaction;
			SetKnockBackValue(player_->GetSystemManager()->GetAttackSystem()->GetKnockBackStrength());
		}

		//ダメージを受ける
		TakeDamage(player_->GetSystemManager()->GetAttackSystem()->GetDamage());

		};
	
	//名前設定
	collider_.name = "Normal" + std::to_string(ID_);
	//刀のコライダー生成
	CreateSwordCollider();

}

void Normal::Update()
{

	preIsStartParryEffect_ = isStartParryEffect_;

	//死亡時、更新しない(別途ステートを作成する予定)
	if (parameter_.hp <= 0.0f) {
		isDead_ = true;
		return;
	}

	//デルタタイムが0.0f以下の時、更新しない
	if (LWP::Info::GetDeltaTime() <= 0.0f) {
		return;
	}

	//パリィエフェクト開始状態ならエフェクトを更新
	if (isStartParryEffect_) {
		UpdateParryEffect();
	}

	//現在の状態を更新
	state_.Update();

	//反発力リセット
	repulsiveForce_ = { 0.0f,0.0f,0.0f };

	//プレイヤーとの距離を計算
	Vector3 diff = GetPlayerPosition() - GetPosition();
	distFromPlayer_ = diff.Length();
}

void Normal::DebugGUI()
{

	if (ImGui::TreeNode(std::to_string(ID_).c_str())) {
		state_.DebugGUI();
		ImGui::Text(std::to_string(distFromPlayer_).c_str());
		ImGui::TreePop();
	}

}

void Normal::AddStateFunc()
{

	state_.init[int(States::kNormalIdle)] = [this](const States& pre) {IdleInit(pre); };
	state_.update[int(States::kNormalIdle)] = [this](std::optional<States>& req, const States& pre) {IdleUpdate(req, pre); };
	state_.finalize[int(States::kNormalIdle)] = [this](const States& pre) {IdleFinalize(pre); };

	state_.init[int(States::kNormalMove)] = [this](const States& pre) {MoveInit(pre); };
	state_.update[int(States::kNormalMove)] = [this](std::optional<States>& req, const States& pre) {MoveUpdate(req, pre); };
	state_.finalize[int(States::kNormalMove)] = [this](const States& pre) {MoveFinalize(pre); };

	state_.init[int(States::kNormalAttack)] = [this](const States& pre) {AttackInit(pre); };
	state_.update[int(States::kNormalAttack)] = [this](std::optional<States>& req, const States& pre) {AttackUpdate(req, pre); };
	state_.finalize[int(States::kNormalAttack)] = [this](const States& pre) {AttackFinalize(pre); };

	state_.init[int(States::kSpacing)] = [this](const States& pre) {SpacingInit(pre); };
	state_.update[int(States::kSpacing)] = [this](std::optional<States>& req, const States& pre) {SpacingUpdate(req, pre); };
	state_.finalize[int(States::kSpacing)] = [this](const States& pre) {SpacingFinalize(pre); };

	state_.init[int(States::kFollowing)] = [this](const States& pre) {FollowingInit(pre); };
	state_.update[int(States::kFollowing)] = [this](std::optional<States>& req, const States& pre) {FollowingUpdate(req, pre); };
	state_.finalize[int(States::kFollowing)] = [this](const States& pre) {FollowingFinalize(pre); };

	state_.init[int(States::kWaitingForAttack)] = [this](const States& pre) {WaitingForAttackInit(pre); };
	state_.update[int(States::kWaitingForAttack)] = [this](std::optional<States>& req, const States& pre) {WaitingForAttackUpdate(req, pre); };
	state_.finalize[int(States::kWaitingForAttack)] = [this](const States& pre) {WaitingForAttackFinalize(pre); };

	state_.init[int(States::kHitReaction)] = [this](const States& pre) {HitReactionInit(pre); };
	state_.update[int(States::kHitReaction)] = [this](std::optional<States>& req, const States& pre) {HitReactionUpdate(req, pre); };
	state_.finalize[int(States::kHitReaction)] = [this](const States& pre) {HitReactionFinalize(pre); };

}
