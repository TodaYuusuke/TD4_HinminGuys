#include "Ogre.h"
#include "../../../../../Player/Player.h"
#include "../../DirectXGame/Engine/primitive/model/Material.h"
#include "../../../../../GameMask.h"
#include "../../../../EnemyManager.h"

using namespace LWP::Primitive;
using namespace GameMask;
using namespace OgreState;

Ogre::Ogre() :
	capsule_(swordCollider_.SetBroadShape(LWP::Object::Collider::Capsule()))
{


}

Ogre::~Ogre()
{



}

void Ogre::Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
	EnemyManager* manager)
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	type_ = EnemyType::kOgre;
	attackType_ = AttackType::kShort;
	//アニメーションロード
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	swordModel_.LoadShortPath("player/SimpleWeapon.gltf");
	model_.materials["Material"].color = { 1.0f,0.0f,0.0f,1.0f };
	SetPlayer(player);
	camera_ = camera;
	enemyManager_ = manager;
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.8f, 0.8f, 0.8f };
	//関数セット
	AddStateFunc();
	state_.request = States::kIdle;

	// 刀モデルをプレイヤーの手に追従させる
	swordModel_.GetJoint("Grip")->localTF.Parent(&model_, "WeaponAnchor");
	// 体の判定生成
	collider_.SetFollow(&model_.worldTF);
	collider_.isActive = true;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	aabb_.min = { -0.4f,-0.5f,-0.4f };
	aabb_.max = { 0.4f,0.5f,0.4f };
	// 自機の所属しているマスクを設定
	collider_.mask.SetBelongFrag(GetEnemy());
	// 当たり判定をとる対象のマスクを設定
	collider_.mask.SetHitFrag(GetAttack());
	collider_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;

		//ステートをセット(攻撃中はリアクションしない)
		if (state_.GetCurrentBehavior() != States::kAssaultSlash) {
			SetPreState(state_.GetCurrentBehavior());
			state_.request = States::kHitReaction;
			//今後プレイヤーから取得する
			SetKnockBackValue(1.0f);
		}

		//ダメージの加算値(テスト用)
		int plusDamage = LWP::Utility::Random::GenerateInt(0, 1000);

		//ダメージエフェクト追加
		//今後プレイヤーから取得する
		enemyManager_->GetDamageEffectEmitter().AddEffect(float(plusDamage),
			model_.GetJointWorldPosition("UpperBody"));

		//ダメージを受ける
		TakeDamage(player_->GetSystemManager()->GetComboTree()->GetDamage());

		};

	//名前設定
	collider_.name = "Ogre" + std::to_string(ID_);
	//刀のコライダー生成
	CreateSwordCollider();

}

void Ogre::Update()
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

void Ogre::DebugGUI()
{

	if (ImGui::TreeNode(std::to_string(ID_).c_str())) {
		state_.DebugGUI();
		ImGui::Text(std::to_string(distFromPlayer_).c_str());
		ImGui::TreePop();
	}

}

void Ogre::CreateSwordCollider()
{
	// 刀の判定生成
	swordCollider_.SetFollow(&model_, "WeaponAnchor");
	swordCollider_.isActive = false;
	// 自機の所属しているマスクを設定
	swordCollider_.mask.SetBelongFrag(GetAttack());
	// 当たり判定をとる対象のマスクを設定
	swordCollider_.mask.SetHitFrag(GetPlayer() | GetParry());
	swordCollider_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		player_->TakeDamage(parameter_.attackParameter.attackValue);
		//判定をオフにする
		//swordCollider_.isActive = false;
		};
	capsule_.radius = 0.1f;
}

void Ogre::AddStateFunc()
{

	state_.init[int(States::kIdle)] = [this](const States& pre) {IdleInit(pre); };
	state_.update[int(States::kIdle)] = [this](std::optional<States>& req, const States& pre) {IdleUpdate(req, pre); };
	state_.finalize[int(States::kIdle)] = [this](const States& pre) {IdleFinalize(pre); };

	state_.init[int(States::kMove)] = [this](const States& pre) {MoveInit(pre); };
	state_.update[int(States::kMove)] = [this](std::optional<States>& req, const States& pre) {MoveUpdate(req, pre); };
	state_.finalize[int(States::kMove)] = [this](const States& pre) {MoveFinalize(pre); };

	state_.init[int(States::kSwingDownAttack)] = [this](const States& pre) {SwingDownAttackInit(pre); };
	state_.update[int(States::kSwingDownAttack)] = [this](std::optional<States>& req, const States& pre) {SwingDownAttackUpdate(req, pre); };
	state_.finalize[int(States::kSwingDownAttack)] = [this](const States& pre) {SwingDownAttackFinalize(pre); };

	state_.init[int(States::kRotatingSlash)] = [this](const States& pre) {RotatingSlashInit(pre); };
	state_.update[int(States::kRotatingSlash)] = [this](std::optional<States>& req, const States& pre) {RotatingSlashUpdate(req, pre); };
	state_.finalize[int(States::kRotatingSlash)] = [this](const States& pre) {RotatingSlashFinalize(pre); };

	state_.init[int(States::kFallingThrust)] = [this](const States& pre) {FallingThrustInit(pre); };
	state_.update[int(States::kFallingThrust)] = [this](std::optional<States>& req, const States& pre) {FallingThrustUpdate(req, pre); };
	state_.finalize[int(States::kFallingThrust)] = [this](const States& pre) {FallingThrustFinalize(pre); };

	state_.init[int(States::kAssaultSlash)] = [this](const States& pre) {AssaultSlashInit(pre); };
	state_.update[int(States::kAssaultSlash)] = [this](std::optional<States>& req, const States& pre) {AssaultSlashUpdate(req, pre); };
	state_.finalize[int(States::kAssaultSlash)] = [this](const States& pre) {AssaultSlashFinalize(pre); };

	state_.init[int(States::kQuadrupleAttack)] = [this](const States& pre) {QuadrupleAttackInit(pre); };
	state_.update[int(States::kQuadrupleAttack)] = [this](std::optional<States>& req, const States& pre) {QuadrupleAttackUpdate(req, pre); };
	state_.finalize[int(States::kQuadrupleAttack)] = [this](const States& pre) {QuadrupleAttackFinalize(pre); };

	state_.init[int(States::kHitReaction)] = [this](const States& pre) {HitReactionInit(pre); };
	state_.update[int(States::kHitReaction)] = [this](std::optional<States>& req, const States& pre) {HitReactionUpdate(req, pre); };
	state_.finalize[int(States::kHitReaction)] = [this](const States& pre) {HitReactionFinalize(pre); };

}
