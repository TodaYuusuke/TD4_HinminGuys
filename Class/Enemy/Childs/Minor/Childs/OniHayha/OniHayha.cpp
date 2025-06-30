#include "OniHayha.h"
#include "../../../../../Player/Player.h"
#include "../../DirectXGame/Engine/primitive/model/Material.h"
#include "../../../../../GameMask.h"
#include "../../../../EnemyManager.h"

using namespace LWP::Primitive;
using namespace GameMask;
using namespace OniHayhaState;

OniHayha::OniHayha(const OniHayhaState::StateParameter& stateParameter)
	: sphere_(bulletCollider_.SetBroadShape(LWP::Object::Collider::Sphere()))
{

	stateParameter_ = stateParameter;

}

OniHayha::~OniHayha()
{

	

}

void OniHayha::Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
	EnemyManager* manager)
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	type_ = EnemyType::kOniHayha;
	attackType_ = AttackType::kLong;
	//アニメーションロード
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	model_.materials["Material"].color = { 1.0f,0.0f,0.0f,1.0f };
	laserModel_.LoadShortPath("effect/laser.obj");
	laserModel_.worldTF.Parent(&model_.worldTF);
	laserModel_.worldTF.scale = {0.01f,0.01f,50.0f};
	laserModel_.worldTF.translation = { 0.0f,1.0f,0.0f };
	laserModel_.materials["Laser"].color.R = (unsigned char)255;
	laserModel_.isActive = false;
	laserModel_.materials["Laser"].enableLighting = false;
	SetPlayer(player);
	camera_ = camera;
	enemyManager_ = manager;
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
	//関数セット
	AddStateFunc();
	state_.request = States::kIdle;

	// 体の判定生成
	collider_.SetFollow(&model_.worldTF);
	collider_.isActive = true;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	aabb_.min = { -0.25f,-0.5f,-0.25f };
	aabb_.max = { 0.25f,0.5f,0.25f };
	// 自機の所属しているマスクを設定
	collider_.mask.SetBelongFrag(GetEnemy());
	// 当たり判定をとる対象のマスクを設定
	collider_.mask.SetHitFrag(GetAttack());
	collider_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;

		//ステートをセット(攻撃中はリアクションしない)
		if (state_.GetCurrentBehavior() != States::kAttack) {
			//ノックバックしたら待機に戻る
			preState_ = States::kIdle;
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
	collider_.name = "OniHayha" + std::to_string(ID_);
	//刀のコライダー生成
	CreateBulletCollider();

}

void OniHayha::Update()
{
	//1フレーム前のパリィエフェクトフラグ更新
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

void OniHayha::DebugGUI()
{

	if (ImGui::TreeNode(std::to_string(ID_).c_str())) {
		state_.DebugGUI();
		ImGui::Text(std::to_string(distFromPlayer_).c_str());
		ImGui::TreePop();
	}

}

void OniHayha::CreateBulletCollider()
{

	// 弾の判定生成
	bulletCollider_.isActive = false;
	// 自機の所属しているマスクを設定
	bulletCollider_.mask.SetBelongFrag(GetAttack());
	// 当たり判定をとる対象のマスクを設定
	bulletCollider_.mask.SetHitFrag(GetPlayer());
	bulletCollider_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		player_->TakeDamage(parameter_.attackParameter.attackValue);
		};
	sphere_.radius = 0.1f;

}

void OniHayha::AddStateFunc()
{

	state_.init[int(States::kIdle)] = [this](const States& pre) {IdleInit(pre); };
	state_.update[int(States::kIdle)] = [this](std::optional<States>& req, const States& pre) {IdleUpdate(req, pre); };
	state_.finalize[int(States::kIdle)] = [this](const States& pre) {IdleFinalize(pre); };

	state_.init[int(States::kAttack)] = [this](const States& pre) {AttackInit(pre); };
	state_.update[int(States::kAttack)] = [this](std::optional<States>& req, const States& pre) {AttackUpdate(req, pre); };
	state_.finalize[int(States::kAttack)] = [this](const States& pre) {AttackFinalize(pre); };

	state_.init[int(States::kRetreat)] = [this](const States& pre) {RetreatInit(pre); };
	state_.update[int(States::kRetreat)] = [this](std::optional<States>& req, const States& pre) {RetreatUpdate(req, pre); };
	state_.finalize[int(States::kRetreat)] = [this](const States& pre) {RetreatFinalize(pre); };

	state_.init[int(States::kWaitingForAttack)] = [this](const States& pre) {WaitingForAttackInit(pre); };
	state_.update[int(States::kWaitingForAttack)] = [this](std::optional<States>& req, const States& pre) {WaitingForAttackUpdate(req, pre); };
	state_.finalize[int(States::kWaitingForAttack)] = [this](const States& pre) {WaitingForAttackFinalize(pre); };

	state_.init[int(States::kHitReaction)] = [this](const States& pre) {HitReactionInit(pre); };
	state_.update[int(States::kHitReaction)] = [this](std::optional<States>& req, const States& pre) {HitReactionUpdate(req, pre); };
	state_.finalize[int(States::kHitReaction)] = [this](const States& pre) {HitReactionFinalize(pre); };

	state_.init[int(States::kAiming)] = [this](const States& pre) {AimingInit(pre); };
	state_.update[int(States::kAiming)] = [this](std::optional<States>& req, const States& pre) {AimingUpdate(req, pre); };
	state_.finalize[int(States::kAiming)] = [this](const States& pre) {AimingFinalize(pre); };

}
