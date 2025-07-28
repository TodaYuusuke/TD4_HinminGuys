#include "ShortFlashes.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

ShortFlashes::ShortFlashes(FollowCamera* followCamera, const std::string& texName) {
	followCamera_ = followCamera;
	texName_ = texName;
}

void ShortFlashes::Initialize() { 

}

void ShortFlashes::Update() {
	// 削除
	particles_.remove_if([](ShortFlash* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<ShortFlash*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void ShortFlashes::SetJsonData(LWP::Utility::JsonIO& json) {
	// 最大サイズ
	json.AddValue<Vector3>("MaxScale", &jsonData_.maxScale);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);
}

void ShortFlashes::Add(int value) {
	for (int i = 0; i < value; i++) {
		ShortFlash* p = new ShortFlash(followCamera_, texName_);
		p->SetShortFlashJsonData(jsonData_);
		p->Create(emitterPos_);
		particles_.push_back(p);
	}
}

void ShortFlashes::Add(int value, LWP::Math::Vector3 position) {
	emitterPos_ = position;
	Add(value);
}
