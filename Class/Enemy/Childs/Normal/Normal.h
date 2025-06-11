#pragma once
#include "../../IEnemy.h"
#include "NormalStateParameter.h"

/// <summary>
/// 雑魚敵クラス
/// </summary>
class Normal : public IEnemy
{
public:
	Normal() = default;
	~Normal() override;

	void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
		EnemyManager* manager) override;

	void Update() override;

	void DebugGUI() override;

	//直前のステートを取得
	States& GetPreState() { return preState_; }
	//直前のステートをセット(HitReactionは除外)
	void SetPreState(States state) { if (state != States::kHitReaction) { preState_ = state; } }

	NormalStateParameter& GetStateParameter() { return stateParameter_; }

private:

	/// <summary>
	/// ステートパターンに使用する関数群追加
	/// </summary>
	void AddStateFunc();

	//ここに各ステートに使う関数追加

	void IdleInit(const States& pre);
	void IdleUpdate(std::optional<States>& req, const States& pre);
	void IdleFinalize(const States& pre);

	void MoveInit(const States& pre);
	void MoveUpdate(std::optional<States>& req, const States& pre);
	void MoveFinalize(const States& pre);

	void AttackInit(const States& pre);
	void AttackUpdate(std::optional<States>& req, const States& pre);
	void AttackFinalize(const States& pre);

	void SpacingInit(const States& pre);
	void SpacingUpdate(std::optional<States>& req, const States& pre);
	void SpacingFinalize(const States& pre);

	void FollowingInit(const States& pre);
	void FollowingUpdate(std::optional<States>& req, const States& pre);
	void FollowingFinalize(const States& pre);

	void WaitingForAttackInit(const States& pre);
	void WaitingForAttackUpdate(std::optional<States>& req, const States& pre);
	void WaitingForAttackFinalize(const States& pre);

	void HitReactionInit(const States& pre);
	void HitReactionUpdate(std::optional<States>& req, const States& pre);
	void HitReactionFinalize(const States& pre);

private:
	
	//設定用パラメータ
	static EnemyParameter configParameter_;

	//雑魚敵パラメータ
	NormalStateParameter stateParameter_;

	//直前のステート保存
	States preState_;

	//状態
	LWP::Utility::StatePattern<States, int(States::kMax)> state_;

};


