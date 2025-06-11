#pragma once
#include "../../IEnemy.h"

/// <summary>
/// ボス敵クラス
/// </summary>
class Boss : public IEnemy
{
public:
	Boss() = default;
	~Boss() override = default;

	void Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
		EnemyManager* manager) override;

	void Update() override;

	void DebugGUI() override;

private:

};


