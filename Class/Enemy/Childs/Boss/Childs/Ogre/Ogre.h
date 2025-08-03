#pragma once
#include "../../Boss.h"
#include "OgreStateParameter.h"
#include "../../../../../Particles/SlashEffect/SlashEffector.h"

/// <summary>
/// 翁雅、降臨
/// </summary>
class Ogre : public Boss
{
public:
	Ogre(OgreState::StateParameter& stateParameter);
	~Ogre() override;

	void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
		EnemyManager* manager) override;

	void Update() override;

	void DebugGUI() override;
	//適用後、強制的に待機状態にさせる
	void ApplyLatestParameter() override { 
		stateParameter_ = configParameter_;
		//SE名前設定
		GetSwingDownAttack().attackData.seFilePath = "enemy/ogre/bossAttack1.mp3";
		GetRotatingSlash().attackData.seFilePath = "enemy/ogre/bossAttack3.mp3";
		GetFallingThrust().attackData.seFilePath = "enemy/ogre/bossAttack1.mp3";
		GetAssaultSlash().attackData.seFilePath = "enemy/ogre/bossAttack2.mp3";
		GetQuadrupleAttack().multipleAttackData[0].seFilePath = "enemy/ogre/bossAttack1.mp3";
		GetQuadrupleAttack().multipleAttackData[1].seFilePath = "enemy/ogre/bossAttack2.mp3";
		GetQuadrupleAttack().multipleAttackData[2].seFilePath = "enemy/ogre/bossAttack1.mp3";
		GetQuadrupleAttack().multipleAttackData[3].seFilePath = "enemy/ogre/bossAttack3.mp3";
		state_.request = OgreState::States::kIdle;
	}

	//直前のステートをセット(HitReactionは除外)
	void SetPreState(OgreState::States state) { if (state != OgreState::States::kHitReaction) { preState_ = state; } }

private:

	//刀のコライダー生成
	void CreateColliders();

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

	void DeadInit(const OgreState::States& pre);
	void DeadUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void DeadFinalize(const OgreState::States& pre);

	void SpawnInit(const OgreState::States& pre);
	void SpawnUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void SpawnFinalize(const OgreState::States& pre);

	void SpawnEnemyInit(const OgreState::States& pre);
	void SpawnEnemyUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void SpawnEnemyFinalize(const OgreState::States& pre);

	void VoidInit(const OgreState::States& pre);
	void VoidUpdate(std::optional<OgreState::States>& req, const OgreState::States& pre);
	void VoidFinalize(const OgreState::States& pre);

	//各パラメータを個別に取得
	OgreState::IdleParameter& GetIdleParameter() { return stateParameter_.idleParameter; }
	OgreState::MoveParameter& GetMoveParameter() { return stateParameter_.moveParameter; }
	OgreState::HitReactionParameter& GetHitReactionParameter() { return stateParameter_.hitReactionParameter; }
	OgreState::SwingDownAttack& GetSwingDownAttack() { return stateParameter_.swingDownAttack; }
	OgreState::RotatingSlash& GetRotatingSlash() { return stateParameter_.rotatingSlash; }
	OgreState::FallingThrust& GetFallingThrust() { return stateParameter_.fallingThrust; }
	OgreState::AssaultSlash& GetAssaultSlash() { return stateParameter_.assaultSlash; }
	OgreState::QuadrupleAttack& GetQuadrupleAttack() { return stateParameter_.quadrupleAttack; }

#pragma endregion
	//攻撃ステートかどうか取得
	bool IsSuperArmorState();
	//連続突撃時のワープ先指定関数
	void SetAssaultSlashWarpPosition();
	//弱攻撃終了時の攻撃抽選
	void EndLightAttack();
	//中攻撃終了時のの攻撃抽選
	void EndMediumAttack();
	//強攻撃終了時の攻撃抽選
	void EndHeavyAttack();

private:

	//翁雅用のデフォパラメータ設定
	OgreState::StateParameter& configParameter_;

	//警告用のプレーンテクスチャ
	LWP::Primitive::NormalSurface cautionQuad_;
	LWP::Primitive::NormalSurface cautionCircle_;

	//斬撃エフェクト
	SlashEffector slashEffector_;

	// 刀モデル
	SkinningModel swordModel_;
	//攻撃コライダー(球)
	LWP::Object::Collision sphereCollider_;
	LWP::Object::Collider::Sphere& sphere_;
	//攻撃コライダー(AABB)
	LWP::Object::Collision aabbAttackCollider_;
	LWP::Object::Collider::AABB& aabbAttack_;

#ifdef _DEBUG

	//一時コライダー表示用
	LWP::Resource::RigidModel box_;
	LWP::Resource::RigidModel tmpSphere_;

#endif // _DEBUG

	//雑魚敵パラメータ
	OgreState::StateParameter stateParameter_;
	//移動後の次のステート格納
	OgreState::States nextAttackState_;
	//直前のステート保存
	OgreState::States preState_;

	//状態
	LWP::Utility::StatePattern<OgreState::States, int(OgreState::States::kMax)> state_;

};


