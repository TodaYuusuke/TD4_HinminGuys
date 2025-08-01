#include "Rings.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

Rings::Rings(const std::string& texName) {
	texName_ = texName;
}

Rings::~Rings() {
	for (Ring* p : particles_) {
		delete p;
	}
	particles_.clear();
}

void Rings::Initialize() {

}

void Rings::Update() {
	// 削除
	particles_.remove_if([](Ring* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<Ring*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void Rings::SetJsonData(LWP::Utility::JsonIO& json) {
	// 最大サイズ
	json.AddValue<Vector3>("MaxScale", &jsonData_.maxScale);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);
}

void Rings::Add(int value) {
	for (int i = 0; i < value; i++) {
		Ring* p = new Ring(texName_);
		p->SetRingJsonData(jsonData_);
		p->Create(emitterPos_);
		particles_.push_back(p);
	}
}

void Rings::Add(int value, LWP::Math::Vector3 position) {
	emitterPos_ = position;
	Add(value);
}
