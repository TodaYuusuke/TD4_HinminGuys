#include "SaijiStateParameter.h"

float SaijiState::IdleParameter::standTime = 1.0f;
float SaijiState::IdleParameter::followingDist = 6.0f;

float SaijiState::MoveParameter::attackDist = 0.5f;
float SaijiState::MoveParameter::runTime = 2.0f;
float SaijiState::MoveParameter::defaultSpeed = 2.0f;

float SaijiState::AttackParameter::endAcceptTime = 0.13f;
float SaijiState::AttackParameter::startAcceptTime = 0.05f;

float SaijiState::SpacingParameter::spacingTime = 2.0f;
float SaijiState::SpacingParameter::spaceDist = 3.0f;

uint16_t SaijiState::WaitingForAttackParameter::attackCount = 0;
uint16_t SaijiState::WaitingForAttackParameter::nextAttackCount = 0;

float SaijiState::FollowingParameter::idleDist = 4.0f;

float SaijiState::HitReactionParameter::decay = 0.9f;

void SaijiState::ParameterConfig::InitJson()
{

	json.Init("SaijiParameter.json");

	json.BeginGroup("SaijiParameter")
		.BeginGroup("Idle")
		.AddValue<float>("StandTime", &SaijiState::IdleParameter::standTime)
		.AddValue<float>("FollowingDist", &SaijiState::IdleParameter::followingDist)
		.EndGroup()
		.BeginGroup("Move")
		.AddValue<float>("AttackDist", &SaijiState::MoveParameter::attackDist)
		.AddValue<float>("RunTime", &SaijiState::MoveParameter::runTime)
		.AddValue<float>("DefaultSpeed", &SaijiState::MoveParameter::defaultSpeed)
		.EndGroup()
		.BeginGroup("Attack")
		.AddValue<float>("StartAcceptTime", &SaijiState::AttackParameter::startAcceptTime)
		.AddValue<float>("EndAcceptTime", &SaijiState::AttackParameter::endAcceptTime)
		.EndGroup()
		.BeginGroup("Spacing")
		.AddValue<float>("SpacingTime", &SaijiState::SpacingParameter::spacingTime)
		.AddValue<float>("SpacingDist", &SaijiState::SpacingParameter::spaceDist)
		.EndGroup()
		.BeginGroup("Following")
		.AddValue<float>("IdleDist", &SaijiState::FollowingParameter::idleDist)
		.EndGroup()
		.BeginGroup("HitReaction")
		.AddValue<float>("Decay", &SaijiState::HitReactionParameter::decay)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

}
