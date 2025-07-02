#include "OgreStateParameter.h"

using namespace LWP::Math;

void OgreState::ParameterConfig::InitJson()
{

	json.Init("OgreParameter.json");

	json.BeginGroup("OgreParameter")
		.BeginGroup("Idle")
		.AddValue<float>("StandTime", &stateParameter_.idleParameter.standTime)
		.AddValue<float>("FollowingDist", &stateParameter_.idleParameter.followingDist)
		.EndGroup()
		.BeginGroup("Move")
		.AddValue<float>("AttackDist", &stateParameter_.moveParameter.attackDist)
		.AddValue<float>("RunTime", &stateParameter_.moveParameter.runTime)
		.AddValue<float>("DefaultSpeed", &stateParameter_.moveParameter.defaultSpeed)
		.AddValue<int32_t>("LightTransitionCount", &stateParameter_.moveParameter.lightTransitionCount)
		.AddValue<int32_t>("MediumTransitionCount", &stateParameter_.moveParameter.mediumTransitionCount)
		.EndGroup()
		.BeginGroup("SwingDownAttack")
		.AddValue<float>("StartAcceptTime", &stateParameter_.swingDownAttack.attackData.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.swingDownAttack.attackData.endAcceptTime)
		.EndGroup()
		.BeginGroup("RotatingSlash")
		.AddValue<float>("StartAcceptTime", &stateParameter_.rotatingSlash.attackData.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.rotatingSlash.attackData.endAcceptTime)
		.EndGroup()
		.BeginGroup("FallingThrust")
		.AddValue<float>("StartAcceptTime", &stateParameter_.fallingThrust.attackData.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.fallingThrust.attackData.endAcceptTime)
		.EndGroup()
		.BeginGroup("AssaultSlash")
		.AddValue<float>("StartAcceptTime", &stateParameter_.assaultSlash.attackData.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.assaultSlash.attackData.endAcceptTime)
		.EndGroup()
		.BeginGroup("QuadrupleAttack");

		for (int32_t i = 0; i < stateParameter_.quadrupleAttack.kMaxAttackCount; i++) {

			std::string numStr = std::to_string(i + 1);

			json.AddValue<float>("StartAcceptTime" + numStr, &stateParameter_.quadrupleAttack.multipleAttackData[i].startAcceptTime)
				.AddValue<float>("EndAcceptTime" + numStr, &stateParameter_.quadrupleAttack.multipleAttackData[i].endAcceptTime)
				.AddValue<float>("EndMotionTime" + numStr, &stateParameter_.quadrupleAttack.multipleAttackData[i].endMotionTime)
				.AddValue<Vector3>("AttackPosition" + numStr, &stateParameter_.quadrupleAttack.multipleAttackData[i].attackPosition)
				.AddValue<float>("AttackScale" + numStr, &stateParameter_.quadrupleAttack.multipleAttackData[i].attackScale)
				.AddValue<float>("MoveSpeed" + numStr, &stateParameter_.quadrupleAttack.multipleAttackData[i].moveSpeed)
				.AddValue<float>("AttackDistance" + numStr, &stateParameter_.quadrupleAttack.multipleAttackData[i].attackDistance);

		}

		json.EndGroup()
		.BeginGroup("HitReaction")
		.AddValue<float>("Decay", &stateParameter_.hitReactionParameter.decay)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

}
