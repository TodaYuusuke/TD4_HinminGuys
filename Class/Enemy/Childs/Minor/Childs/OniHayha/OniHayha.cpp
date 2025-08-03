#include "OniHayha.h"
#include "../../../../../Player/Player.h"
#include "../../../../../GameMask.h"
#include "../../../../EnemyManager.h"
#include "../../../../../Audio/SEPlayer.h"

using namespace LWP::Primitive;
using namespace GameMask;
using namespace OniHayhaState;

OniHayha::OniHayha(OniHayhaState::StateParameter& stateParameter, float delayTime) :
	configParameter_(stateParameter),
	capsule_(bulletCollider_.SetBroadShape(LWP::Object::Collider::Capsule()))
{

	stateParameter_ = stateParameter;
	stateParameter_.spawnParameter.currentSpawnWaitingTime = delayTime;

}

OniHayha::~OniHayha()
{

	

}

void OniHayha::Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
	EnemyManager* manager)
{

#ifdef _DEBUG
	tmpSphere_.LoadSphere();
	tmpSphere_.isActive = false;
	tmpSphereSecond_.LoadSphere();
	tmpSphereSecond_.isActive = false;
#endif // _DEBUG

	model_.LoadShortPath("Oniheihe/Oniheihe_IK.gltf");
	type_ = EnemyType::kOniHayha;
	attackType_ = AttackType::kLong;
	//アニメーションロード
	animation_.LoadFullPath("resources/model/Oniheihe/Oniheihe_IK.gltf", &model_);
	gunModel_.LoadShortPath("Oniheihe/MatchLockGun.gltf");
	// 銃モデルをプレイヤーの手に追従させる
	gunModel_.GetJoint("Grip")->localTF.Parent(&model_, "WeaponAnchor");
	laserModel_.LoadShortPath("effect/laser.obj");
	laserModel_.worldTF.Parent(&model_.worldTF);
	laserModel_.worldTF.translation.y = 0.5f;
	/*laserModel_.worldTF.translation = { 0.0f,1.0f,0.0f };*/
	laserModel_.materials["Laser"].color.R = (unsigned char)255;
	laserModel_.isActive = false;
	laserModel_.materials["Laser"].enableLighting = false;
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
		sePlayer_->PlaySE("enemy/attack_5.mp3", "hit", 1.0f);

		//ステートをセット(攻撃中はリアクションしない)
		if (state_.GetCurrentBehavior() != States::kAttack) {
			//ノックバックしたら待機に戻る
			preState_ = States::kIdle;
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

		// ダメージパーティクル
		enemyManager_->GetParticles()->CreateAttackHitEffect(model_.worldTF.GetWorldPosition() + Vector3{ 0.0f, 0.5f, 0.0f });

		//ダメージを受ける
		TakeDamage(resultDamage);

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

	//反発力リセット
	repulsiveForce_ = { 0.0f,0.0f,0.0f };

	//プレイヤーとの距離を計算
	Vector3 diff = GetPlayerPosition() - GetPosition();
	distFromPlayer_ = diff.Length();

	// 足音再生
	SoundFootOnGround();
}

void OniHayha::DebugGUI()
{

	if (ImGui::TreeNode(std::to_string(ID_).c_str())) {
		state_.DebugGUI();
		ImGui::Text(std::to_string(distFromPlayer_).c_str());
		ImGui::Text("HP: %1.2f", parameter_.hp);
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
	bulletCollider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		player_->TakeDamage(parameter_.attackParameter.attackValue);
		};
	capsule_.radius = 0.1f;

#ifdef _DEBUG

	tmpSphere_.worldTF.scale = { 0.1f,0.1f,0.1f };
	tmpSphereSecond_.worldTF.scale = { 0.1f,0.1f,0.1f };

#endif // _DEBUG


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

	state_.init[int(States::kDead)] = [this](const States& pre) {DeadInit(pre); };
	state_.update[int(States::kDead)] = [this](std::optional<States>& req, const States& pre) {DeadUpdate(req, pre); };
	state_.finalize[int(States::kDead)] = [this](const States& pre) {DeadFinalize(pre); };

	state_.init[int(States::kSpawn)] = [this](const States& pre) {SpawnInit(pre); };
	state_.update[int(States::kSpawn)] = [this](std::optional<States>& req, const States& pre) {SpawnUpdate(req, pre); };
	state_.finalize[int(States::kSpawn)] = [this](const States& pre) {SpawnFinalize(pre); };

}
