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
