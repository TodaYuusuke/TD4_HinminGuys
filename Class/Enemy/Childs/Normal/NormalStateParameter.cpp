#include "NormalStateParameter.h"

int32_t IdleParameter::standTime_ = 60;
float IdleParameter::followingDist_ = 6.0f;

float MoveParameter::attackDist_ = 0.5f;
int32_t MoveParameter::runTime_ = 120;

float AttackParameter::attackAcceptTime_ = 0.13f;

int32_t SpacingParameter::spacingTime_ = 120;
float SpacingParameter::spaceDist_ = 3.0f;

uint16_t WaitingForAttackParameter::attackCount_ = 0;
uint16_t WaitingForAttackParameter::nextAttackCount_ = 0;

float FollowingParameter::idleDist_ = 4.0f;

float HitReactionParameter::decay_ = 0.9f;
