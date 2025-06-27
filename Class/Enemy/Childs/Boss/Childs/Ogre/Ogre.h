#pragma once
#include "../../Boss.h"
#include "OgreStateParameter.h"

/// <summary>
/// オニ・ヘイヘ~青い死神~
/// </summary>
class Ogre : public Boss
{
public:
	Ogre();
	~Ogre() override;

	void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
		EnemyManager* manager) override;

	void Update() override;

	void DebugGUI() override;

	//直前のステートをセット(HitReactionは除外)
	void SetPreState(OgreState::States state) { if (state != OgreState::States::kHitReaction) { preState_ = state; } }

private:

	//刀のコライダー生成
	void CreateSwordCollider();

	/// <summary>
	/// ステートパターンに使用する関数群追加
	/// </summary>
	void AddStateFunc();

	//ここに各ステートに使う関数追加
#pragma region ステートパターン関数追加

	void IdleInit(const OgreState::States& pre);
	void IdleUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void IdleFinalize(const OgreState::States& pre);

	void MoveInit(const OgreState::States& pre);
	void MoveUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void MoveFinalize(const OgreState::States& pre);

	void SwingDownAttackInit(const OgreState::States& pre);
	void SwingDownAttackUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void SwingDownAttackFinalize(const OgreState::States& pre);

	void RotatingSlashInit(const OgreState::States& pre);
	void RotatingSlashUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void RotatingSlashFinalize(const OgreState::States& pre);

	void FallingThrustInit(const OgreState::States& pre);
	void FallingThrustUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void FallingThrustFinalize(const OgreState::States& pre);

	void AssaultSlashInit(const OgreState::States& pre);
	void AssaultSlashUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void AssaultSlashFinalize(const OgreState::States& pre);

	void QuadrupleAttackInit(const OgreState::States& pre);
	void QuadrupleAttackUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void QuadrupleAttackFinalize(const OgreState::States& pre);

	void HitReactionInit(const OgreState::States& pre);
	void HitReactionUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void HitReactionFinalize(const OgreState::States& pre);

#pragma endregion

private:

	// 刀モデル
	SkinningModel swordModel_;
	// 刀コライダー
	LWP::Object::Collision swordCollider_;
	LWP::Object::Collider::Capsule& capsule_;
	//弾の攻撃方向
	Vector3 bulletDirection_{};

	//雑魚敵パラメータ
	OgreState::StateParameter stateParameter_;

	//直前のステート保存
	OgreState::States preState_;

	//状態
	LWP::Utility::StatePattern<OgreState::States, int(OgreState::States::kMax)> state_;

};


