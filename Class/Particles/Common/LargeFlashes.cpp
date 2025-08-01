#include "LargeFlashes.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

LargeFlashes::LargeFlashes(FollowCamera* followCamera, const std::string& texName) {
	followCamera_ = followCamera;
	texName_ = texName;
}

LargeFlashes::~LargeFlashes() {
	for (LargeFlash* p : particles_) {
		delete p;
	}
	particles_.clear();
}

void LargeFlashes::Initialize() {

}

void LargeFlashes::Update() {
	// 削除
	particles_.remove_if([](LargeFlash* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<LargeFlash*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void LargeFlashes::SetJsonData(LWP::Utility::JsonIO& json) {
	json.BeginGroup("LargeFlash");
	// 最大サイズ
	json.AddValue<Vector3>("MaxScale", &jsonData_.maxScale);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);

	json.EndGroup();
}

void LargeFlashes::Add(int value) {
	for (int i = 0; i < value; i++) {
		LargeFlash* p = new LargeFlash(followCamera_, texName_);
		p->SetLargeFlashJsonData(jsonData_);
		p->Create(emitterPos_);
		particles_.push_back(p);
	}
}

void LargeFlashes::Add(int value, LWP::Math::Vector3 position) {
	emitterPos_ = position;
	Add(value);
}