#pragma once
#include "../../Minor.h"
#include "OniHayhaStateParameter.h"

/// <summary>
/// オニ・ヘイヘ~青い死神~
/// </summary>
class OniHayha : public Minor
{
public:
	OniHayha() = default;
	~OniHayha() override;

	void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
		EnemyManager* manager) override;

	void Update() override;

	void DebugGUI() override;

	//直前のステートをセット(HitReactionは除外)
	void SetPreState(OniHayhaState::States state) { if (state != OniHayhaState::States::kHitReaction) { preState_ = state; } }

private:

	/// <summary>
	/// ステートパターンに使用する関数群追加
	/// </summary>
	void AddStateFunc();

	//ここに各ステートに使う関数追加

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

private:

	//雑魚敵パラメータ
	OniHayhaState::StateParameter stateParameter_;

	//直前のステート保存
	OniHayhaState::States preState_;

	//状態
	LWP::Utility::StatePattern<OniHayhaState::States, int(OniHayhaState::States::kMax)> state_;

};


