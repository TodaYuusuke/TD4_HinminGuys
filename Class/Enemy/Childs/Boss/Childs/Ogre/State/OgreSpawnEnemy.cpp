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

}

void Ogre::SpawnEnemyUpdate(std::optional<States>& req, const States& pre)
{

	//アニメーションが終了した時
	if (not animation_.GetPlaying()) {
		//移動状態に移行
		state_.request = States::kMove;
		return;
	}

}
