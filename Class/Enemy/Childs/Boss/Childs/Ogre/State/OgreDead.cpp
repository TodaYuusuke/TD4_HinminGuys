#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

using namespace OgreState;

void Ogre::DeadFinalize(const States& pre)
{
}

void Ogre::DeadInit(const States& pre)
{

	animation_.Play("Dead", 0.2f)
		.Loop(false);
	animation_.GetPlayBackSpeed() = 1.0f;
	// 死亡パーティクル生成
	Vector3 pos = {
		model_.worldTF.GetWorldPosition().x,
		0.5f,
		model_.worldTF.GetWorldPosition().z,
	};
	enemyManager_->GetParticles()->CreateBossDeadParticles(animation_.GetTotalSeconds(LWP::Resource::Animation::TrackType::Main), pos);

}

void Ogre::DeadUpdate(std::optional<States>& req, const States& pre)
{
	Vector3 pos = {
		model_.worldTF.GetWorldPosition().x,
		0.5f,
		model_.worldTF.GetWorldPosition().z,
	};
	enemyManager_->GetParticles()->SetBossDeadParticlesPos(pos);
	if ((animation_.GetProgress() >= stateParameter_.deadParameter.backStartF and
		animation_.GetProgress() < stateParameter_.deadParameter.backEndF) or
		(animation_.GetProgress() >= stateParameter_.deadParameter.backStartS and
			animation_.GetProgress() < stateParameter_.deadParameter.backEndS)) {

		Vector3 velocity = Vector3{ 0.0f,0.0f,1.0f } *stateParameter_.deadParameter.stepBackSpeed;
		velocity *= forward_;

		SetPosition(GetPosition() + velocity * LWP::Info::GetDeltaTimeF());

	}

	//アニメーションが終了した時
	if (not animation_.GetPlaying()) {
		//死亡フラグを立てる
		isDead_ = true;
		//クリアフラグをオンにする
		enemyManager_->SetIsDefeatedOgre(true);
	}

}
