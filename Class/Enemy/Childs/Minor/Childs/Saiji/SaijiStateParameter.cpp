#include "SaijiStateParameter.h"

void SaijiState::ParameterConfig::InitJson()
{

	json.Init("SaijiParameter.json");

	json.BeginGroup("SaijiParameter")
		.BeginGroup("Idle")
		.AddValue<float>("StandTime", &stateParameter_.idleParameter.standTime)
		.AddValue<float>("FollowingDist", &stateParameter_.idleParameter.followingDist)
		.AddValue<float>("StartStandTime", &stateParameter_.idleParameter.countStandTime)
		.EndGroup()
		.BeginGroup("Move")
		.AddValue<float>("AttackDist", &stateParameter_.moveParameter.attackDist)
		.AddValue<float>("RunTime", &stateParameter_.moveParameter.runTime)
		.AddValue<float>("DefaultSpeed", &stateParameter_.moveParameter.defaultSpeed)
		.EndGroup()
		.BeginGroup("Attack")
		.AddValue<float>("StartAcceptTime", &stateParameter_.attackParameter.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.attackParameter.endAcceptTime)
		.EndGroup()
		.BeginGroup("Spacing")
		.AddValue<float>("SpacingTime", &stateParameter_.spacingParameter.spacingTime)
		.AddValue<float>("SpacingDist", &stateParameter_.spacingParameter.spaceDist)
		.EndGroup()
		.BeginGroup("Following")
		.AddValue<float>("IdleDist", &stateParameter_.followingParameter.idleDist)
		.EndGroup()
		.BeginGroup("HitReaction")
		.AddValue<float>("Decay", &stateParameter_.hitReactionParameter.decay)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

}
