#include "NormalStateParameter.h"

float IdleParameter::standTime = 1.0f;
float IdleParameter::followingDist = 6.0f;

float MoveParameter::attackDist = 0.5f;
float MoveParameter::runTime = 2.0f;
float MoveParameter::defaultSpeed = 2.0f;

float AttackParameter::endAcceptTime = 0.13f;
float AttackParameter::startAcceptTime = 0.05f;

float SpacingParameter::spacingTime = 2.0f;
float SpacingParameter::spaceDist = 3.0f;

uint16_t WaitingForAttackParameter::attackCount = 0;
uint16_t WaitingForAttackParameter::nextAttackCount = 0;

float FollowingParameter::idleDist = 4.0f;

float HitReactionParameter::decay = 0.9f;
