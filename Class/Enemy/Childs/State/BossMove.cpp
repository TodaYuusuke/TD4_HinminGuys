#include "BossMove.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void BossMove::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
}

void BossMove::Update()
{
}

void BossMove::DebugGUI()
{

	if (ImGui::TreeNode("BossMove")) {
		ImGui::TreePop();
	}

}
