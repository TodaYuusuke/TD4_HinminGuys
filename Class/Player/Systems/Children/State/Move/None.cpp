#include "None.h"
#include "../../../../Player.h"

None::None(Move* moveSystem, Player* player) {
	moveSystem_ = moveSystem;
	player_ = player;

	// ダッシュ状態解除
	player_->GetSystemManager()->GetEvasionSystem()->SetIsDash(false);
}

void None::Initialize()
{
}

void None::Update()
{
}

void None::AnimCommand()
{
}
