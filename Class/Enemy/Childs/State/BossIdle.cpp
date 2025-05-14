#include "BossIdle.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void BossIdle::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
}

void BossIdle::Update()
{
}

void BossIdle::DebugGUI()
{

	if (ImGui::TreeNode("BossIdle")) {
		ImGui::TreePop();
	}

}
