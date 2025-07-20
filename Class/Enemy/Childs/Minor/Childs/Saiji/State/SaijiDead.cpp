#include "../Saiji.h"
#include "../../../../../EnemyManager.h"

using namespace SaijiState;

void Saiji::DeadFinalize(const States& pre)
{
}

void Saiji::DeadInit(const States& pre)
{

	animation_.Play("Dead", 0.2f)
		.Loop(false);

}

void Saiji::DeadUpdate(std::optional<States>& req, const States& pre)
{

	//アニメーションが終了した時
	if (not animation_.GetPlaying()) {
		//死亡フラグを立てる
		isDead_ = true;

	}

}
