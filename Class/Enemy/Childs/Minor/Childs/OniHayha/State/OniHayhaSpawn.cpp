#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace OniHayhaState;

void OniHayha::SpawnFinalize(const States& pre)
{
	collider_.isActive = true;
}

void OniHayha::SpawnInit(const States& pre)
{
	animation_.Play("Idle", 0.6f)
		.Loop(true);
	collider_.isActive = false;
	stateParameter_.spawnParameter.currentTime = 0.0f;
	model_.worldTF.translation.y = stateParameter_.spawnParameter.startY;
}

void OniHayha::SpawnUpdate(std::optional<States>& req, const States& pre)
{

	//Y座標だけ移動させる
	model_.worldTF.translation.y = LWP::Utility::Interpolation::LerpF(stateParameter_.spawnParameter.startY, 0.0f,
		stateParameter_.spawnParameter.currentTime / stateParameter_.spawnParameter.appearanceTime);

	//スポーン時間終わるまでカウント
	if (stateParameter_.spawnParameter.currentTime < stateParameter_.spawnParameter.appearanceTime) {

		stateParameter_.spawnParameter.currentTime += 1.0f * LWP::Info::GetDeltaTimeF();

		//超過したら待機状態に移行
		if (stateParameter_.spawnParameter.currentTime >= stateParameter_.spawnParameter.appearanceTime) {
			model_.worldTF.translation.y = 0.0f;
			state_.request = States::kIdle;
			return;
		}

	}

}
