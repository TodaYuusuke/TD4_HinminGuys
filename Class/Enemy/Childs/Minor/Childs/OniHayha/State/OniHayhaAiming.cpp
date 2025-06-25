#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace OniHayhaState;

void OniHayha::AimingFinalize(const OniHayhaState::States& pre) {

	laserModel_.isActive = false;

}

void OniHayha::AimingInit(const OniHayhaState::States& pre) {

	laserModel_.isActive = true;
	isAttack_ = true;
	stateParameter_.aimingParameter.flickeringCounter = AimingParameter::flickeringInterval;

}

void OniHayha::AimingUpdate(std::optional<OniHayhaState::States>& req, const OniHayhaState::States& pre) {

	//カウントダウン
	if (stateParameter_.aimingParameter.countAimingTime > 0) {
		stateParameter_.aimingParameter.countAimingTime -= 1.0f * LWP::Info::GetDeltaTimeF();
	}

	//0になったら状態切り替え
	if (stateParameter_.aimingParameter.countAimingTime <= 0) {
		//攻撃状態に移行
		state_.request = States::kAttack;
		return;
	}

	//チカチカタァァァーーイム！！！残り時間が少なくなるとチカチカするぜ！！
	if (stateParameter_.aimingParameter.countAimingTime < stateParameter_.aimingParameter.flickeringTime) {

		//チカチカカウントを減少
		if (stateParameter_.aimingParameter.flickeringCounter > 0) {
			stateParameter_.aimingParameter.flickeringCounter--;
		}
		//0になったらアクティブ切り替え
		if (stateParameter_.aimingParameter.flickeringCounter <= 0) {
			laserModel_.isActive = not laserModel_.isActive;
			stateParameter_.aimingParameter.flickeringCounter = AimingParameter::flickeringInterval;
		}

	}

	//プレイヤーの向きに回転
	RotateTowardsPlayer();

}
