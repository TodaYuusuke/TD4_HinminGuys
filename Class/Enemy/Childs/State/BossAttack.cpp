#include "BossAttack.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void BossAttack::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
}

void BossAttack::Update()
{
}

void BossAttack::DebugGUI()
{

	if (ImGui::TreeNode("BossAttack")) {
		ImGui::TreePop();
	}

}
