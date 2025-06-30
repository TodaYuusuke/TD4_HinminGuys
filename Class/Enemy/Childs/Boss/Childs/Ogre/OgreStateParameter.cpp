#include "OgreStateParameter.h"

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
		.AddValue<float>("StartAcceptTime", &stateParameter_.swingDownAttack.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.swingDownAttack.endAcceptTime)
		.EndGroup()
		.BeginGroup("RotatingSlash")
		.AddValue<float>("StartAcceptTime", &stateParameter_.rotatingSlash.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.rotatingSlash.endAcceptTime)
		.EndGroup()
		.BeginGroup("FallingThrust")
		.AddValue<float>("StartAcceptTime", &stateParameter_.fallingThrust.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.fallingThrust.endAcceptTime)
		.EndGroup()
		.BeginGroup("AssaultSlash")
		.AddValue<float>("StartAcceptTime", &stateParameter_.assaultSlash.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.assaultSlash.endAcceptTime)
		.EndGroup()
		.BeginGroup("QuadrupleAttack")
		.AddValue<float>("StartAcceptTime", &stateParameter_.quadrupleAttack.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.quadrupleAttack.endAcceptTime)
		.EndGroup()
		.BeginGroup("HitReaction")
		.AddValue<float>("Decay", &stateParameter_.hitReactionParameter.decay)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

}
