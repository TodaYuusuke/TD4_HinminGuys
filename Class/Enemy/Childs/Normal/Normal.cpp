#include "Normal.h"
#include "../../../Player/Player.h"
#include "State/NormalIdle.h"
#include "../../DirectXGame/Engine/primitive/model/Material.h"
#include "../../../GameMask.h"

using namespace LWP::Primitive;
using namespace GameMask;

Normal::~Normal()
{

	//ステートが存在していたら削除
	if (state_) {
		delete state_;
	}

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
	state_ = new NormalIdle(this);
	state_->Initialize();
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
		if (state_->GetState() != States::kNormalAttack) {
			SetPreState(state_->GetState());
			SetState(States::kHitReaction);
			SetKnockBackValue(player_->GetSystemManager()->GetComboTree()->GetNockBackStrength());
		}

		//ダメージを受ける
		TakeDamage(player_->GetParameter()->GetCurrentAttackStrength());

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
	state_->Update();

	//反発力リセット
	repulsiveForce_ = { 0.0f,0.0f,0.0f };

	//プレイヤーとの距離を計算
	Vector3 diff = GetPlayerPosition() - GetPosition();
	distFromPlayer_ = diff.Length();
}

void Normal::SetState(States state, bool isInit)
{

	//前回の状態を開放、新しい状態に置き換える
	if (state_) {
		delete state_;
	}

	switch (state)
	{
	case States::kNormalIdle:
		state_ = new NormalIdle(this);
		break;
	case States::kNormalMove:
		state_ = new NormalMove(this);
		break;
	case States::kNormalAttack:
		state_ = new NormalAttack(this);
		break;
	case States::kSpacing:
		state_ = new Spacing(this);
		break;
	case States::kFollowing:
		state_ = new Following(this);
		break;
	case States::kWaitingForAttack:
		state_ = new WaitingForAttack(this);
		break;
	case States::kHitReaction:
		state_ = new HitReaction(this);
		break;
	default:
		break;
	}

	//初期化フラグが立っているなら初期化
	if (isInit) {
		//初期化
		state_->Initialize();
	}

}

