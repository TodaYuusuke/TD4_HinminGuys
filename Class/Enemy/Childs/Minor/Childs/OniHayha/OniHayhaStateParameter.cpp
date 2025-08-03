#include "OniHayhaStateParameter.h"

void OniHayhaState::ParameterConfig::InitJson()
{

	json.Init("OniHayhaParameter.json");

	json.BeginGroup("OniHayhaParameter")
		.AddValue<float>("ExtrusionDist", &stateParameter_.extrusionDist)
		.BeginGroup("Idle")
		.AddValue<float>("StandTime", &stateParameter_.idleParameter.standTime)
		.AddValue<float>("RetreatDist", &stateParameter_.idleParameter.retreatDist)
		.EndGroup()
		.BeginGroup("Attack")
		.AddValue<float>("StartAcceptTime", &stateParameter_.attackParameter.startAcceptTime)
		.AddValue<float>("EndAcceptTime", &stateParameter_.attackParameter.endAcceptTime)
		.AddValue<float>("BulletSpeed", &stateParameter_.attackParameter.bulletSpeed)
		.AddValue<float>("SEPlayTime", &stateParameter_.attackParameter.sePlayTime)
		.EndGroup()
		.BeginGroup("Retreat")
		.AddValue<float>("IdleDist", &stateParameter_.retreatParameter.idleDist)
		.EndGroup()
		.BeginGroup("HitReaction")
		.AddValue<float>("Decay", &stateParameter_.hitReactionParameter.decay)
		.EndGroup()
		.BeginGroup("Aiming")
		.AddValue<float>("AimingTime", &stateParameter_.aimingParameter.aimingTime)
		.AddValue<float>("FlickeringTime", &stateParameter_.aimingParameter.flickeringTime)
		.AddValue<int32_t>("FlickeringInterval", &stateParameter_.aimingParameter.flickeringInterval)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

}
