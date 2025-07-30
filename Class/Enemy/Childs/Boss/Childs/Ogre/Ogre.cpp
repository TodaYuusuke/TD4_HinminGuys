#include "Ogre.h"
#include "../../../../../Player/Player.h"
#include "../../../../../GameMask.h"
#include "../../../../EnemyManager.h"
#include "../../../../../Audio/SEPlayer.h"

using namespace LWP::Primitive;
using namespace GameMask;
using namespace OgreState;

Ogre::Ogre(OgreState::StateParameter& stateParameter) :
	configParameter_(stateParameter),
	sphere_(sphereCollider_.SetBroadShape(LWP::Object::Collider::Sphere())),
	aabbAttack_(aabbAttackCollider_.SetBroadShape(LWP::Object::Collider::AABB()))
{

	stateParameter_ = stateParameter;

}

Ogre::~Ogre()
{



}

void Ogre::Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
	EnemyManager* manager)
{
	model_.LoadShortPath("Ogre/Orga_IK.gltf");
	cautionQuad_.LoadTexture("caution_square.png");
	cautionCircle_.LoadTexture("caution_circle.png");
	cautionQuad_.worldTF.translation.y = 0.1f;
	cautionQuad_.worldTF.rotation = Quaternion::ConvertEuler(Vector3{ 1.57f, 0.0f,0.0f });
	cautionQuad_.isActive = false;
	cautionCircle_.worldTF.translation.y = 0.2f;
	cautionCircle_.worldTF.rotation = Quaternion::ConvertEuler(Vector3{ 1.57f, 0.0f,0.0f });
	cautionCircle_.isActive = false;
	type_ = EnemyType::kOgre;
	attackType_ = AttackType::kShort;
	//アニメーションロード
	animation_.LoadFullPath("resources/model/Ogre/Orga_IK.gltf", &model_);
	swordModel_.LoadShortPath("Ogre/Weapon.gltf");
	model_.materials["Material"].color = { 1.0f,0.0f,0.0f,1.0f };
	SetPlayer(player);
	camera_ = camera;
	enemyManager_ = manager;
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.8f, 0.8f, 0.8f };
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
	aabbBody_.min = { -0.4f,-0.5f,-0.4f };
	aabbBody_.max = { 0.4f,0.5f,0.4f };
	// 自機の所属しているマスクを設定
	collider_.mask.SetBelongFrag(GetEnemy());
	// 当たり判定をとる対象のマスクを設定
	collider_.mask.SetHitFrag(GetAttack());
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;

		//SE鳴らす
		sePlayer_->PlaySE("attack_5.mp3", "hit", 1.0f);

		//ステートをセット(攻撃中はリアクションしない)
		if (not IsAttackState()) {
			state_.request = States::kHitReaction;
			//今後プレイヤーから取得する
			SetKnockBackValue(1.0f);
		}

		//コライダーを一時的にオフ、クールタイム設定
		collider_.isActive = false;
		//プレイヤーから取得してくる
		invincibleTime_ = 0.2f;

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
	CreateColliders();

	//弱攻撃の2つの中で抽選し、0ならふり降ろし
	if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
		nextAttackState_ = States::kSwingDownAttack;
	}
	//1なら回転切り
	else {
		nextAttackState_ = States::kRotatingSlash;
	}

}

void Ogre::Update()
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

bool Ogre::IsAttackState()
{

	if (state_.GetCurrentBehavior() == States::kSwingDownAttack or
		state_.GetCurrentBehavior() == States::kRotatingSlash or
		state_.GetCurrentBehavior() == States::kFallingThrust or
		state_.GetCurrentBehavior() == States::kAssaultSlash or
		state_.GetCurrentBehavior() == States::kQuadrupleAttack) {
		return true;
	}

	return false;
}

void Ogre::CreateColliders()
{
	
	// 球の判定生成
	sphereCollider_.SetFollow(&model_, "Hips");
	sphereCollider_.isActive = false;
	// 自機の所属しているマスクを設定
	sphereCollider_.mask.SetBelongFrag(GetAttack());
	// 当たり判定をとる対象のマスクを設定
	sphereCollider_.mask.SetHitFrag(GetPlayer() | GetParry());
	
	sphereCollider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		player_->TakeDamage(parameter_.attackParameter.attackValue);
		//判定をオフにする
		//swordCollider_.isActive = false;
		};
	// AABBの判定生成
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

#ifdef _DEBUG
	sphere_.isShowWireFrame = false;
	aabbAttack_.isShowWireFrame = false;
#endif // _DEBUG

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

	state_.init[int(States::kDead)] = [this](const States& pre) {DeadInit(pre); };
	state_.update[int(States::kDead)] = [this](std::optional<States>& req, const States& pre) {DeadUpdate(req, pre); };
	state_.finalize[int(States::kDead)] = [this](const States& pre) {DeadFinalize(pre); };

	state_.init[int(States::kSpawn)] = [this](const States& pre) {SpawnInit(pre); };
	state_.update[int(States::kSpawn)] = [this](std::optional<States>& req, const States& pre) {SpawnUpdate(req, pre); };
	state_.finalize[int(States::kSpawn)] = [this](const States& pre) {SpawnFinalize(pre); };

}

void Ogre::EndLightAttack()
{

	//カウント加算
	stateParameter_.moveParameter.lightAttackCount++;

	//カウントが設定数以上になったら、次の攻撃抽選を変更
	if (stateParameter_.moveParameter.lightAttackCount >= stateParameter_.moveParameter.lightTransitionCount) {

		//カウントリセット
		stateParameter_.moveParameter.lightAttackCount = 0;

		//抽選して、0の場合中攻撃
		if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
			//落下攻撃に移行する
			nextAttackState_ = States::kFallingThrust;
		}
		//強攻撃
		else {

			//強攻撃の2つの中で抽選し、0なら連続突撃
			if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
				nextAttackState_ = States::kAssaultSlash;
			}
			//1なら四連撃
			else {
				nextAttackState_ = States::kQuadrupleAttack;
			}

		}

	}
	//弱攻撃抽選
	else {

		//弱攻撃の2つの中で抽選し、0ならふり降ろし
		if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
			nextAttackState_ = States::kSwingDownAttack;
		}
		//1なら回転切り
		else {
			nextAttackState_ = States::kRotatingSlash;
		}

	}

}

void Ogre::EndMediumAttack()
{

	//中攻撃カウント増加
	stateParameter_.moveParameter.mediumAttackCount++;
	//カウントが設定数以上になったら、次の攻撃抽選を変更
	if (stateParameter_.moveParameter.mediumAttackCount >= stateParameter_.moveParameter.mediumTransitionCount) {

		//カウントリセット
		stateParameter_.moveParameter.mediumAttackCount = 0;

		//強攻撃の2つの中で抽選し、0なら連続突撃
		if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
			nextAttackState_ = States::kAssaultSlash;
		}
		//1なら四連撃
		else {
			nextAttackState_ = States::kQuadrupleAttack;
		}

	}
	//弱攻撃抽選
	else {

		//弱攻撃の2つの中で抽選し、0ならふり降ろし
		if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
			nextAttackState_ = States::kSwingDownAttack;
		}
		//1なら回転切り
		else {
			nextAttackState_ = States::kRotatingSlash;
		}

	}

}

void Ogre::EndHeavyAttack()
{

	//弱攻撃の2つの中で抽選し、0ならふり降ろし
	if (LWP::Utility::Random::GenerateInt(0, 1) == 0) {
		nextAttackState_ = States::kSwingDownAttack;
	}
	//1なら回転切り
	else {
		nextAttackState_ = States::kRotatingSlash;
	}

}
