#include "ParameterEditor.h"

using namespace EnemyConfig;

void ParameterEditor::Initialize() {

	parameterJson_.Init("EnemyParameters.json");

	parameterJson_.BeginGroup("EnemyParameters");

	for (int32_t i = 0; i < int(EnemyType::kMax); i++) {

		parameterJson_.BeginGroup(IEnemy::enemyTypeName[i])
			.AddValue("HP", &parameters_[i].maxHp)
			.AddValue("AttackValue", &parameters_[i].attackParameter.attackValue)
			.AddValue("KnockbackValue", &parameters_[i].attackParameter.knockbackValue)
			.EndGroup();

	}
	parameterJson_.AddValue<float>("ParryEffectOccurTime", &parryEffectOccurTime_);
	parameterJson_.EndGroup();

}

void ParameterEditor::DebugGUI() {

	if (ImGui::TreeNode("Enemy Parameters")) {

		parameterJson_.DebugGUI();

		ImGui::TreePop();

	}

}
