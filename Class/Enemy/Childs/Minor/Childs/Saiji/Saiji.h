#pragma once
#include "../../Minor.h"
#include "SaijiStateParameter.h"

/// <summary>
/// ダメだよぉ～才二君
/// </summary>
class Saiji : public Minor
{
public:
	Saiji() = default;
	~Saiji() override;

	void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
		EnemyManager* manager) override;

	void Update() override;

	void DebugGUI() override;

	//直前のステートをセット(HitReactionは除外)
	void SetPreState(SaijiState::States state) { if (state != SaijiState::States::kHitReaction) { preState_ = state; } }

private:

	/// <summary>
	/// ステートパターンに使用する関数群追加
	/// </summary>
	void AddStateFunc();

	//ここに各ステートに使う関数追加

	void IdleInit(const SaijiState::States& pre);
	void IdleUpdate(std::optional<SaijiState::States>& req, const SaijiState::States& pre);
	void IdleFinalize(const SaijiState::States& pre);

	void MoveInit(const SaijiState::States& pre);
	void MoveUpdate(std::optional<SaijiState::States>& req, const SaijiState::States& pre);
	void MoveFinalize(const SaijiState::States& pre);

	void AttackInit(const SaijiState::States& pre);
	void AttackUpdate(std::optional<SaijiState::States>& req, const SaijiState::States& pre);
	void AttackFinalize(const SaijiState::States& pre);

	void SpacingInit(const SaijiState::States& pre);
	void SpacingUpdate(std::optional<SaijiState::States>& req, const SaijiState::States& pre);
	void SpacingFinalize(const SaijiState::States& pre);

	void FollowingInit(const SaijiState::States& pre);
	void FollowingUpdate(std::optional<SaijiState::States>& req, const SaijiState::States& pre);
	void FollowingFinalize(const SaijiState::States& pre);

	void WaitingForAttackInit(const SaijiState::States& pre);
	void WaitingForAttackUpdate(std::optional<SaijiState::States>& req, const SaijiState::States& pre);
	void WaitingForAttackFinalize(const SaijiState::States& pre);

	void HitReactionInit(const SaijiState::States& pre);
	void HitReactionUpdate(std::optional<SaijiState::States>& req, const SaijiState::States& pre);
	void HitReactionFinalize(const SaijiState::States& pre);

private:

	//才二君用のデフォパラメータ設定
	static EnemyParameter configParameter_;

	//雑魚敵パラメータ
	SaijiState::StateParameter stateParameter_;

	//直前のステート保存
	SaijiState::States preState_;

	//状態
	LWP::Utility::StatePattern<SaijiState::States, int(SaijiState::States::kMax)> state_;

};


