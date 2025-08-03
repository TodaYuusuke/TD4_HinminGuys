#include "../Ogre.h"
#include "../../../../../EnemyManager.h"

using namespace OgreState;

void Ogre::SpawnEnemyFinalize(const States& pre)
{
	
}

void Ogre::SpawnEnemyInit(const States& pre)
{
	animation_.Play("ShadowHide", 0.6f)
		.Loop(false);

	if (stateParameter_.spawnEnemy.currentCount == 0) {
		enemyManager_->CreateEnemy(GetPosition() + Vector3{ 5.0f,0.0f,0.0f }, EnemyType::kSaiji);
		enemyManager_->CreateEnemy(GetPosition() + Vector3{ -5.0f,0.0f,0.0f }, EnemyType::kSaiji);
		stateParameter_.spawnEnemy.currentCount++;
	}
	else if(stateParameter_.spawnEnemy.currentCount == 1) {
		enemyManager_->CreateEnemy(GetPosition() + Vector3{ 5.0f,0.0f,0.0f }, EnemyType::kSaiji);
		enemyManager_->CreateEnemy(GetPosition() + Vector3{ -5.0f,0.0f,0.0f }, EnemyType::kSaiji);
		enemyManager_->CreateEnemy(GetPosition() + Vector3{ 0.0f,0.0f,-5.0f }, EnemyType::kSaiji);
		enemyManager_->CreateEnemy(GetPosition() + Vector3{ 0.0f,0.0f,5.0f }, EnemyType::kOniHayha);
		stateParameter_.spawnEnemy.currentCount++;
	}

	stateParameter_.spawnEnemy.isPlayedSE = false;

}

void Ogre::SpawnEnemyUpdate(std::optional<States>& req, const States& pre)
{

	//SEを鳴らす時間に到達したら鳴らす
	if (animation_.GetProgress() >= stateParameter_.spawnEnemy.sePlayTime and 
		not stateParameter_.spawnEnemy.isPlayedSE) {
		sePlayer_->PlaySE("enemy/ogre/piercing.mp3", "piercing", 0.5f);
		stateParameter_.spawnEnemy.isPlayedSE = true;
	}

	//アニメーションが終了した時
	if (not animation_.GetPlaying()) {
		//移動状態に移行
		state_.request = States::kMove;
		return;
	}

}
