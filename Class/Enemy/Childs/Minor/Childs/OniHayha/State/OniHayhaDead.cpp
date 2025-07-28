#include "../OniHayha.h"
#include "../../../../../EnemyManager.h"

using namespace OniHayhaState;

void OniHayha::DeadFinalize(const States& pre)
{
}

void OniHayha::DeadInit(const States& pre)
{

	animation_.Play("Dead", 0.2f)
		.Loop(false);

}

void OniHayha::DeadUpdate(std::optional<States>& req, const States& pre)
{

	//アニメーションが終了した時
	if (not animation_.GetPlaying()) {
		//死亡フラグを立てる
		isDead_ = true;

	}

}
