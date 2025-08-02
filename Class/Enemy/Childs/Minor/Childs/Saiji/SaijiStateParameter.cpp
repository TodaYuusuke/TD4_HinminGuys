#include "SaijiStateParameter.h"

using namespace LWP::Math;

void SaijiState::ParameterConfig::InitJson()
{

	json.Init("SaijiParameter.json");

	json.BeginGroup("SaijiParameter")
		.AddValue<float>("ExtrusionDist", &stateParameter_.extrusionDist)
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
		.AddValue<Vector3>("AttackPosition", &stateParameter_.attackParameter.attackPosition)
		.AddValue<float>("AttackScale", &stateParameter_.attackParameter.attackScale)
		.AddValue<float>("FreezingTime", &stateParameter_.attackParameter.freezingTime)
		.AddValue<Vector3>("EffectPosition", &stateParameter_.attackParameter.effectParam.position)
		.AddValue<Vector3>("EffectRotate", &stateParameter_.attackParameter.effectParam.rotate)
		.AddValue<Vector3>("EffectScale", &stateParameter_.attackParameter.effectParam.scale)
		.AddValue<float>("EffectPlayTime", &stateParameter_.attackParameter.effectParam.playTime)
		.AddValue<Vector3>("EffectOffset", &stateParameter_.attackParameter.effectParam.offset)
		.AddValue<LWP::Utility::Color>("EffectColor", &stateParameter_.attackParameter.effectParam.color)
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
