#pragma once
#include "../../Minor.h"
#include "FasterStateParameter.h"

/// <summary>
/// 雑魚敵クラス
/// </summary>
class Faster : public Minor
{
public:
	Faster() = default;
	~Faster() override;

	void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
		EnemyManager* manager) override;

	void Update() override;

	void DebugGUI() override;

	//直前のステートをセット(HitReactionは除外)
	void SetPreState(FasterState::States state) { if (state != FasterState::States::kHitReaction) { preState_ = state; } }

private:

	/// <summary>
	/// ステートパターンに使用する関数群追加
	/// </summary>
	void AddStateFunc();

	//ここに各ステートに使う関数追加

	void IdleInit(const FasterState::States& pre);
	void IdleUpdate(std::optional<FasterState::States>& req, const FasterState::States& pre);
	void IdleFinalize(const FasterState::States& pre);

	void MoveInit(const FasterState::States& pre);
	void MoveUpdate(std::optional<FasterState::States>& req, const FasterState::States& pre);
	void MoveFinalize(const FasterState::States& pre);

	void AttackInit(const FasterState::States& pre);
	void AttackUpdate(std::optional<FasterState::States>& req, const FasterState::States& pre);
	void AttackFinalize(const FasterState::States& pre);

	void SpacingInit(const FasterState::States& pre);
	void SpacingUpdate(std::optional<FasterState::States>& req, const FasterState::States& pre);
	void SpacingFinalize(const FasterState::States& pre);

	void FollowingInit(const FasterState::States& pre);
	void FollowingUpdate(std::optional<FasterState::States>& req, const FasterState::States& pre);
	void FollowingFinalize(const FasterState::States& pre);

	void WaitingForAttackInit(const FasterState::States& pre);
	void WaitingForAttackUpdate(std::optional<FasterState::States>& req, const FasterState::States& pre);
	void WaitingForAttackFinalize(const FasterState::States& pre);

	void HitReactionInit(const FasterState::States& pre);
	void HitReactionUpdate(std::optional<FasterState::States>& req, const FasterState::States& pre);
	void HitReactionFinalize(const FasterState::States& pre);

private:

	//雑魚敵パラメータ
	FasterState::StateParameter stateParameter_;

	//直前のステート保存
	FasterState::States preState_;

	//状態
	LWP::Utility::StatePattern<FasterState::States, int(FasterState::States::kMax)> state_;

};


