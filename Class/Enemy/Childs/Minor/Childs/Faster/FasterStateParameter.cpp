#include "FasterStateParameter.h"

float FasterState::IdleParameter::standTime = 1.0f;
float FasterState::IdleParameter::followingDist = 6.0f;

float FasterState::MoveParameter::attackDist = 0.5f;
float FasterState::MoveParameter::runTime = 2.0f;
float FasterState::MoveParameter::defaultSpeed = 2.0f;

float FasterState::AttackParameter::endAcceptTime = 0.13f;
float FasterState::AttackParameter::startAcceptTime = 0.05f;

float FasterState::SpacingParameter::spacingTime = 2.0f;
float FasterState::SpacingParameter::spaceDist = 3.0f;

uint16_t FasterState::WaitingForAttackParameter::attackCount = 0;
uint16_t FasterState::WaitingForAttackParameter::nextAttackCount = 0;

float FasterState::FollowingParameter::idleDist = 4.0f;

float FasterState::HitReactionParameter::decay = 0.9f;
