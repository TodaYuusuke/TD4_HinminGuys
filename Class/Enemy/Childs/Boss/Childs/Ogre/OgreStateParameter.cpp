#include "OgreStateParameter.h"

using namespace LWP::Math;

void OgreState::ParameterConfig::InitJson()
{

	json.Init("OgreParameter.json");

	json.BeginGroup("OgreParameter")
		.AddValue<float>("ExtrusionDist", &stateParameter_.extrusionDist)
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
		.AddValue<float>("EndMotionTime", &stateParameter_.swingDownAttack.attackData.endMotionTime)
		.AddValue<Vector3>("AttackPosition", &stateParameter_.swingDownAttack.attackData.attackPosition)
		.AddValue<float>("AttackScale", &stateParameter_.swingDownAttack.attackData.attackScale)
		.AddValue<float>("MoveSpeed", &stateParameter_.swingDownAttack.attackData.moveSpeed)
		.AddValue<float>("AttackDistance", &stateParameter_.swingDownAttack.attackData.attackDistance)
		.EndGroup()
		.BeginGroup("RotatingSlash")
		.AddValue<float>("StartAcceptTime", &stateParameter_.rotatingSlash.attackData.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.rotatingSlash.attackData.endAcceptTime)
		.AddValue<float>("EndMotionTime", &stateParameter_.rotatingSlash.attackData.endMotionTime)
		.AddValue<Vector3>("AttackPosition", &stateParameter_.rotatingSlash.attackData.attackPosition)
		.AddValue<float>("AttackScale", &stateParameter_.rotatingSlash.attackData.attackScale)
		.AddValue<float>("MoveSpeed", &stateParameter_.rotatingSlash.attackData.moveSpeed)
		.AddValue<float>("AttackDistance", &stateParameter_.rotatingSlash.attackData.attackDistance)
		.EndGroup()
		.BeginGroup("FallingThrust")
		.AddValue<float>("StartAcceptTime", &stateParameter_.fallingThrust.attackData.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.fallingThrust.attackData.endAcceptTime)
		.AddValue<float>("EndMotionTime", &stateParameter_.fallingThrust.attackData.endMotionTime)
		.AddValue<Vector3>("AttackPosition", &stateParameter_.fallingThrust.attackData.attackPosition)
		.AddValue<float>("AttackScale", &stateParameter_.fallingThrust.attackData.attackScale)
		.AddValue<float>("JumpingTime", &stateParameter_.fallingThrust.jumpingTime)
		.AddValue<float>("FallWaitingTime", &stateParameter_.fallingThrust.fallWaitingTime)
		.AddValue<float>("FallingTime", &stateParameter_.fallingThrust.fallingTime)
		.AddValue<float>("AttackTime", &stateParameter_.fallingThrust.attackTime)
		.AddValue<float>("GapTime", &stateParameter_.fallingThrust.gapTime)
		.AddValue<float>("JumpingHeight", &stateParameter_.fallingThrust.jumpingHeight)
		.EndGroup()
		.BeginGroup("AssaultSlash")
		.AddValue<Vector3>("AttackPosition", &stateParameter_.assaultSlash.attackData.attackPosition)
		.AddValue<float>("AttackScale", &stateParameter_.assaultSlash.attackData.attackScale)
		.AddValue<float>("LeaveDistance", &stateParameter_.assaultSlash.leaveDistance)
		.AddValue<float>("OverDistance", &stateParameter_.assaultSlash.overDistance)
		.AddValue<float>("DivingTime", &stateParameter_.assaultSlash.divingTime)
		.AddValue<float>("StealthWaitingTime", &stateParameter_.assaultSlash.stealthWaitingTime)
		.AddValue<float>("WaitingTime", &stateParameter_.assaultSlash.waitingTime)
		.AddValue<float>("AssaultTime", &stateParameter_.assaultSlash.assaultTime)
		.AddValue<float>("GapTime", &stateParameter_.assaultSlash.gapTime)
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
