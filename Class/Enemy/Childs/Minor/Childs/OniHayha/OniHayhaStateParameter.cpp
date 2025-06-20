#include "OniHayhaStateParameter.h"

float OniHayhaState::IdleParameter::standTime = 1.0f;
float OniHayhaState::IdleParameter::retreatDist = 1.0f;

float OniHayhaState::AttackParameter::endAcceptTime = 0.13f;
float OniHayhaState::AttackParameter::startAcceptTime = 0.05f;

float OniHayhaState::RetreatParameter::idleDist = 3.0f;

uint16_t OniHayhaState::WaitingForAttackParameter::attackCount = 0;
uint16_t OniHayhaState::WaitingForAttackParameter::nextAttackCount = 0;

float OniHayhaState::HitReactionParameter::decay = 0.9f;
