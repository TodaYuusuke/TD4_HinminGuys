#include "IEffect.h"
#include "../../Camera/FollowCamera.h"
#include "../Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

void IEffect::Update() {
	for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		// 生存時間が過ぎたら処理を行わない
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		// 各パーティクルの更新処理
		//(*particleIterator).updateFunc(*particleIterator);

		// 生存時間
		(*particleIterator).currentTime++;

		particleIterator++;
	}
}
