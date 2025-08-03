#pragma once
#include "../../Minor.h"
#include "OniHayhaStateParameter.h"

/// <summary>
/// オニ・ヘイヘ~青い死神~
/// </summary>
class OniHayha : public Minor
{
public:
	OniHayha(OniHayhaState::StateParameter& stateParameter, float delayTime);
	~OniHayha() override;

	void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
		EnemyManager* manager) override;

	void Update() override;

	void DebugGUI() override;
	//適用後、強制的に待機状態にさせる
	void ApplyLatestParameter() override { 
		stateParameter_ = configParameter_;
		state_.request = OniHayhaState::States::kIdle;
	}

	//直前のステートをセット(HitReactionは除外)
	void SetPreState(OniHayhaState::States state) { if (state != OniHayhaState::States::kHitReaction) { preState_ = state; } }

private:

	//弾のコライダー生成
	void CreateBulletCollider();

	/// <summary>
	/// ステートパターンに使用する関数群追加
	/// </summary>
	void AddStateFunc();

	//ここに各ステートに使う関数追加
#pragma region ステートパターン関数追加

	void IdleInit(const OniHayhaState::States& pre);
	void IdleUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre);
	void IdleFinalize(const OniHayhaState::States& pre);

	void AttackInit(const OniHayhaState::States& pre);
	void AttackUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre);
	void AttackFinalize(const OniHayhaState::States& pre);

	void RetreatInit(const OniHayhaState::States& pre);
	void RetreatUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre);
	void RetreatFinalize(const OniHayhaState::States& pre);

	void WaitingForAttackInit(const OniHayhaState::States& pre);
	void WaitingForAttackUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre);
	void WaitingForAttackFinalize(const OniHayhaState::States& pre);

	void HitReactionInit(const OniHayhaState::States& pre);
	void HitReactionUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre);
	void HitReactionFinalize(const OniHayhaState::States& pre);

	void AimingInit(const OniHayhaState::States& pre);
	void AimingUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre);
	void AimingFinalize(const OniHayhaState::States& pre);

	void DeadInit(const OniHayhaState::States& pre);
	void DeadUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre);
	void DeadFinalize(const OniHayhaState::States& pre);

	void SpawnInit(const OniHayhaState::States& pre);
	void SpawnUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre);
	void SpawnFinalize(const OniHayhaState::States& pre);

#pragma endregion

private:

	//オニ・ヘイヘ用のデフォパラメータ設定
	OniHayhaState::StateParameter& configParameter_;

	//警告用のレーザー
	LWP::Resource::RigidModel laserModel_;

	// 銃モデル
	SkinningModel gunModel_;
	// 弾コライダー
	LWP::Object::Collision bulletCollider_;
	LWP::Object::Collider::Capsule& capsule_;
	//弾の攻撃方向
	Vector3 bulletDirection_{};

#ifdef _DEBUG

	//一時コライダー表示用
	LWP::Resource::RigidModel tmpSphere_;
	//一時コライダー表示用
	LWP::Resource::RigidModel tmpSphereSecond_;

#endif // _DEBUG

	//雑魚敵パラメータ
	OniHayhaState::StateParameter stateParameter_;

	//直前のステート保存
	OniHayhaState::States preState_;

	//状態
	LWP::Utility::StatePattern<OniHayhaState::States, int(OniHayhaState::States::kMax)> state_;

};


