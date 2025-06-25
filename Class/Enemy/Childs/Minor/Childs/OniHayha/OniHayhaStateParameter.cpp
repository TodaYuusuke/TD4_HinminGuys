#include "OniHayhaStateParameter.h"

float OniHayhaState::IdleParameter::standTime = 3.0f;
float OniHayhaState::IdleParameter::retreatDist = 3.0f;

float OniHayhaState::AttackParameter::endAcceptTime = 1.0f;
float OniHayhaState::AttackParameter::startAcceptTime = 0.05f;
float OniHayhaState::AttackParameter::bulletSpeed = 20.0f;

float OniHayhaState::RetreatParameter::idleDist = 5.0f;

uint16_t OniHayhaState::WaitingForAttackParameter::attackCount = 0;
uint16_t OniHayhaState::WaitingForAttackParameter::nextAttackCount = 0;

float OniHayhaState::HitReactionParameter::decay = 0.9f;

float OniHayhaState::AimingParameter::aimingTime = 2.5f;
float OniHayhaState::AimingParameter::flickeringTime = 1.0f;
int32_t OniHayhaState::AimingParameter::flickeringInterval = 3;

void OniHayhaState::ParameterConfig::InitJson()
{

	json.Init("OniHayhaParameter.json");

	json.BeginGroup("OniHayhaParameter")
		.BeginGroup("Idle")
		.AddValue<float>("StandTime", &OniHayhaState::IdleParameter::standTime)
		.AddValue<float>("RetreatDist", &OniHayhaState::IdleParameter::retreatDist)
		.EndGroup()
		.BeginGroup("Attack")
		.AddValue<float>("StartAcceptTime", &OniHayhaState::AttackParameter::startAcceptTime)
		.AddValue<float>("EndAcceptTime", &OniHayhaState::AttackParameter::endAcceptTime)
		.AddValue<float>("BulletSpeed", &OniHayhaState::AttackParameter::bulletSpeed)
		.EndGroup()
		.BeginGroup("Retreat")
		.AddValue<float>("IdleDist", &OniHayhaState::RetreatParameter::idleDist)
		.EndGroup()
		.BeginGroup("HitReaction")
		.AddValue<float>("Decay", &OniHayhaState::HitReactionParameter::decay)
		.EndGroup()
		.BeginGroup("Aiming")
		.AddValue<float>("AimingTime", &OniHayhaState::AimingParameter::aimingTime)
		.AddValue<float>("FlickeringTime", &OniHayhaState::AimingParameter::flickeringTime)
		.AddValue<int32_t>("FlickeringInterval", &OniHayhaState::AimingParameter::flickeringInterval)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

}
