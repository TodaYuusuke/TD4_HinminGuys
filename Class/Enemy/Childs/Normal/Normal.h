#pragma once
#include "../../IEnemy.h"
#include "INormalEnemyState.h"
#include "NormalStateParameter.h"
#include "State/States.h"

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
	//ステートセット(enum版)
	void SetState(States state, bool isInit = true);
	//直前のステートを取得
	States& GetPreState() { return preState_; }
	//直前のステートをセット(HitReactionは除外)
	void SetPreState(States state) { if (state != States::kHitReaction) { preState_ = state; } }

	NormalStateParameter& GetStateParameter() { return stateParameter_; }

private:
	
	//設定用パラメータ
	static EnemyParameter configParameter_;

	//雑魚敵パラメータ
	NormalStateParameter stateParameter_;

	//直前のステート保存
	States preState_;

	//状態
	INormalEnemyState* state_;

};


