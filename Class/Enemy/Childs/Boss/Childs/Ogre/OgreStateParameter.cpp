#include "OgreStateParameter.h"

float OgreState::IdleParameter::standTime = 1.0f;
float OgreState::IdleParameter::followingDist = 6.0f;

float OgreState::MoveParameter::attackDist = 0.5f;
float OgreState::MoveParameter::runTime = 2.0f;
float OgreState::MoveParameter::defaultSpeed = 2.0f;
int32_t OgreState::MoveParameter::lightTransitionCount = 2;
int32_t OgreState::MoveParameter::mediumTransitionCount = 1;

float OgreState::SwingDownAttack::startAcceptTime = 0.05f;
float OgreState::SwingDownAttack::endAcceptTime = 0.13f;

float OgreState::RotatingSlash::startAcceptTime = 0.05f;
float OgreState::RotatingSlash::endAcceptTime = 0.13f;

float OgreState::FallingThrust::startAcceptTime = 0.05f;
float OgreState::FallingThrust::endAcceptTime = 0.13f;

float OgreState::AssaultSlash::startAcceptTime = 0.05f;
float OgreState::AssaultSlash::endAcceptTime = 0.13f;

float OgreState::QuadrupleAttack::startAcceptTime = 0.05f;
float OgreState::QuadrupleAttack::endAcceptTime = 0.13f;

float OgreState::HitReactionParameter::decay = 0.9f;

void OgreState::ParameterConfig::InitJson()
{

	json.Init("OgreParameter.json");

	json.BeginGroup("OgreParameter")
		.BeginGroup("Idle")
		.AddValue<float>("StandTime", &OgreState::IdleParameter::standTime)
		.AddValue<float>("FollowingDist", &OgreState::IdleParameter::followingDist)
		.EndGroup()
		.BeginGroup("Move")
		.AddValue<float>("AttackDist", &OgreState::MoveParameter::attackDist)
		.AddValue<float>("RunTime", &OgreState::MoveParameter::runTime)
		.AddValue<float>("DefaultSpeed", &OgreState::MoveParameter::defaultSpeed)
		.AddValue<int32_t>("LightTransitionCount", &OgreState::MoveParameter::lightTransitionCount)
		.AddValue<int32_t>("MediumTransitionCount", &OgreState::MoveParameter::mediumTransitionCount)
		.EndGroup()
		.BeginGroup("SwingDownAttack")
		.AddValue<float>("StartAcceptTime", &OgreState::SwingDownAttack::startAcceptTime)
		.AddValue<float>("EndAcceptTime", &OgreState::SwingDownAttack::endAcceptTime)
		.EndGroup()
		.BeginGroup("RotatingSlash")
		.AddValue<float>("StartAcceptTime", &OgreState::RotatingSlash::startAcceptTime)
		.AddValue<float>("EndAcceptTime", &OgreState::RotatingSlash::endAcceptTime)
		.EndGroup()
		.BeginGroup("FallingThrust")
		.AddValue<float>("StartAcceptTime", &OgreState::FallingThrust::startAcceptTime)
		.AddValue<float>("EndAcceptTime", &OgreState::FallingThrust::endAcceptTime)
		.EndGroup()
		.BeginGroup("AssaultSlash")
		.AddValue<float>("StartAcceptTime", &OgreState::AssaultSlash::startAcceptTime)
		.AddValue<float>("EndAcceptTime", &OgreState::AssaultSlash::endAcceptTime)
		.EndGroup()
		.BeginGroup("QuadrupleAttack")
		.AddValue<float>("StartAcceptTime", &OgreState::QuadrupleAttack::startAcceptTime)
		.AddValue<float>("EndAcceptTime", &OgreState::QuadrupleAttack::endAcceptTime)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

}
