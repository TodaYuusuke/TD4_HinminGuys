#include "Saiji.h"
#include "../../../../../Player/Player.h"
#include "../../../../../GameMask.h"
#include "../../../../EnemyManager.h"
#include "../../../../../Audio/SEPlayer.h"

using namespace LWP::Primitive;
using namespace GameMask;
using namespace SaijiState;

Saiji::Saiji(SaijiState::StateParameter& stateParameter) :
	configParameter_(stateParameter),
	aabbAttack_(aabbAttackCollider_.SetBroadShape(LWP::Object::Collider::AABB())),
	slashEffector_("Effect/SwordSlash.png", { 256.0f, 256.0f }, 26)
{

	stateParameter_ = stateParameter;

}

Saiji::~Saiji()
{

	

}

void Saiji::Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
	EnemyManager* manager)
{

#ifdef _DEBUG
	box_.LoadCube();
	box_.isActive = false;
#endif // _DEBUG

	model_.LoadShortPath("Saiji/Saiji_IK.gltf");
	type_ = EnemyType::kSaiji;
	attackType_ = AttackType::kShort;
	//アニメーションロード
	animation_.LoadFullPath("resources/model/Saiji/Saiji_IK.gltf", &model_);
	swordModel_.LoadShortPath("Saiji/Club.gltf");
	SetPlayer(player);
	camera_ = camera;
	enemyManager_ = manager;
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
	//プレイヤーの向きに回転
	RotateTowardsPlayer();
	//関数セット
	AddStateFunc();
	state_.request = States::kSpawn;
	model_.worldTF.translation.y = stateParameter_.spawnParameter.startY;

	// 刀モデルをプレイヤーの手に追従させる
	swordModel_.GetJoint("Grip")->localTF.Parent(&model_, "WeaponAnchor");
	// 体の判定生成
	collider_.SetFollow(&model_.worldTF);
	collider_.isActive = true;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	aabbBody_.min = { -0.25f,-0.5f,-0.25f };
	aabbBody_.max = { 0.25f,0.5f,0.25f };
	// 自機の所属しているマスクを設定
	collider_.mask.SetBelongFrag(GetEnemy());
	// 当たり判定をとる対象のマスクを設定
	collider_.mask.SetHitFrag(GetAttack());
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;

		//SE鳴らす
		sePlayer_->PlaySE("attack_5.mp3", "hit", 1.0f);

		//ステートをセット(攻撃中はリアクションしない)
		if (state_.GetCurrentBehavior() != States::kAttack) {
			state_.request = States::kHitReaction;
			//今後プレイヤーから取得する
			SetKnockBackValue(1.0f);
		}

		//コライダーを一時的にオフ、クールタイム設定
		collider_.isActive = false;

		//鞘の場合、専用のクールタイム設定
		if (hitTarget->name == "Sheath") {
			//クールタイム設定
			invincibleTime_ = 1.01f;
		}
		else {
			// 攻撃力
			player_->GetParameter()->attackStrength_ = player_->GetSystemManager()->GetComboTree()->GetDamage();
			//プレイヤーから取得し、0の場合が無いよう極小のクールタイムを足す
			invincibleTime_ = player_->GetSystemManager()->GetComboTree()->GetHitCoolTime() + 0.01f;
		}

		//ダメージの倍率
		float mag = LWP::Utility::Random::GenerateFloat(0.96f, 1.11f);

		float resultDamage = player_->GetParameter()->GetCurrentAttackStrength() * mag;

		//ダメージエフェクト追加
		//今後プレイヤーから取得する
		enemyManager_->GetDamageEffectEmitter().AddEffect(resultDamage,
			model_.GetJointWorldPosition("UpperBody"));

		//ダメージを受ける
		TakeDamage(resultDamage);

		};
	
	//名前設定
	collider_.name = "Saiji" + std::to_string(ID_);
	//刀のコライダー生成
	CreateSwordCollider();

	slashEffector_.SetParentTF(&model_.worldTF);

}

void Saiji::Update()
{

	preIsStartParryEffect_ = isStartParryEffect_;

	//無敵時間カウント
	if (invincibleTime_ > 0.0f) {

		invincibleTime_ -= 1.0f * LWP::Info::GetDeltaTimeF();
		//カウントが終わったらコライダーオン
		if (invincibleTime_ <= 0.0f) {
			collider_.isActive = true;
		}

	}

	//死亡時
	if (parameter_.hp <= 0.0f) {
		//コライダーオフ
		collider_.isActive = false;
		//死亡ステートでなければ強制的に死亡ステートに移行
		if (state_.GetCurrentBehavior() != States::kDead) {
			state_.request = States::kDead;
		}
		
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

	//エフェクト更新
	slashEffector_.Update();

	//反発力リセット
	repulsiveForce_ = { 0.0f,0.0f,0.0f };

	//プレイヤーとの距離を計算
	Vector3 diff = GetPlayerPosition() - GetPosition();
	distFromPlayer_ = diff.Length();
}

void Saiji::DebugGUI()
{

	if (ImGui::TreeNode(std::to_string(ID_).c_str())) {
		state_.DebugGUI();
		ImGui::Text(std::to_string(distFromPlayer_).c_str());
		ImGui::Text("HP: %1.2f", parameter_.hp);
		ImGui::TreePop();
	}

}

void Saiji::CreateSwordCollider()
{
	// 刀の判定生成
	aabbAttackCollider_.SetFollow(&model_, "Hips");
	aabbAttackCollider_.isActive = false;
	// 自機の所属しているマスクを設定
	aabbAttackCollider_.mask.SetBelongFrag(GetAttack());
	// 当たり判定をとる対象のマスクを設定
	aabbAttackCollider_.mask.SetHitFrag(GetPlayer() | GetParry());
	aabbAttackCollider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		player_->TakeDamage(parameter_.attackParameter.attackValue);
		//判定をオフにする
		//swordCollider_.isActive = false;
		};
}

void Saiji::AddStateFunc()
{

	state_.init[int(States::kIdle)] = [this](const States& pre) {IdleInit(pre); };
	state_.update[int(States::kIdle)] = [this](std::optional<States>& req, const States& pre) {IdleUpdate(req, pre); };
	state_.finalize[int(States::kIdle)] = [this](const States& pre) {IdleFinalize(pre); };

	state_.init[int(States::kMove)] = [this](const States& pre) {MoveInit(pre); };
	state_.update[int(States::kMove)] = [this](std::optional<States>& req, const States& pre) {MoveUpdate(req, pre); };
	state_.finalize[int(States::kMove)] = [this](const States& pre) {MoveFinalize(pre); };

	state_.init[int(States::kAttack)] = [this](const States& pre) {AttackInit(pre); };
	state_.update[int(States::kAttack)] = [this](std::optional<States>& req, const States& pre) {AttackUpdate(req, pre); };
	state_.finalize[int(States::kAttack)] = [this](const States& pre) {AttackFinalize(pre); };

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

	state_.init[int(States::kDead)] = [this](const States& pre) {DeadInit(pre); };
	state_.update[int(States::kDead)] = [this](std::optional<States>& req, const States& pre) {DeadUpdate(req, pre); };
	state_.finalize[int(States::kDead)] = [this](const States& pre) {DeadFinalize(pre); };

	state_.init[int(States::kSpawn)] = [this](const States& pre) {SpawnInit(pre); };
	state_.update[int(States::kSpawn)] = [this](std::optional<States>& req, const States& pre) {SpawnUpdate(req, pre); };
	state_.finalize[int(States::kSpawn)] = [this](const States& pre) {SpawnFinalize(pre); };

}
