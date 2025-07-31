#include "MoveEffect.h"
#include "../../../../../Camera/FollowCamera.h"
#include "../../../../../Player/Player.h"
#include "../../../../Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

MoveEffect::MoveEffect(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;
}

void MoveEffect::Initialize() {
	//disappearEasing = Utility::Easing::kFunction[]
}

void MoveEffect::Update() {
	//maxWhite = std::clamp<int>(maxWhite, 1, 255);
	//minWhite = std::clamp<int>(minWhite, 0, 254);
	//maxAlpha = std::clamp<int>(maxAlpha, 1, 255);
	//minAlpha = std::clamp<int>(minAlpha, 0, 254);
	//IEffect::Update();
	for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		// 生存時間が過ぎたら処理を行わない
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		// 各パーティクルの更新処理
		//if ((*particleIterator).updateFunc) {
			//(*particleIterator).updateFunc(*particleIterator);
		//}

		// 生存時間
		(*particleIterator).currentTime++;

		particleIterator++;
	}
}

void MoveEffect::DebugGui() {
	//maxColor = { maxWhite, maxWhite,maxWhite,maxAlpha };
	//minColor = { minWhite, minWhite,maxWhite,maxAlpha };

	//maxWhite = std::clamp<int>(maxWhite, 0, 255);
	//minWhite = std::clamp<int>(minWhite, 0, 254);
	//maxAlpha = std::clamp<int>(maxAlpha, 0, 255);
	//minAlpha = std::clamp<int>(minAlpha, 0, 254);

	//// どれが選ばれているかを示すインデックス（0 から始まる）
	//static int selectedIndex = 0;

	//// Comboボックスの表示
	//if (ImGui::Combo("Easing Type", &selectedIndex, Easing::kTypeNames, IM_ARRAYSIZE(Easing::kTypeNames))) {
	//	// 選択が変更されたときの処理（必要があればここに書く）
	//}

	//// 使用例：選ばれた名前を取得する
	//const char* selectedName = Easing::kTypeNames[selectedIndex];
	//disappearEasing = Easing::kFunction[selectedIndex];
}

void MoveEffect::SetJsonData(LWP::Utility::JsonIO& json) {
#pragma region Circle
	json.BeginGroup("Circle");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &dustCloudData_.velocity.max);
	json.AddValue<Vector3>("Min", &dustCloudData_.velocity.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<Vector3>("Max", &dustCloudData_.scale.max);
	json.AddValue<Vector3>("Min", &dustCloudData_.scale.min);
	json.EndGroup();
	// 色
	json.BeginGroup("Color");
	json.BeginGroup("White");
	json.AddValue<int>("Max", &maxWhite);
	json.AddValue<int>("Min", &minWhite);
	json.EndGroup();
	json.BeginGroup("Alpha");
	json.AddValue<int>("Max", &maxAlpha);
	json.AddValue<int>("Min", &minAlpha);
	json.EndGroup();
	json.BeginGroup("CheckMin,Max");
	json.AddValue<Color>("Max", &maxColor);
	json.AddValue<Color>("Min", &minColor);
	json.EndGroup();
	json.EndGroup();

	json.EndGroup();

	// 初速
	json.AddValue<Vector3>("First", &dustCloudData_.firstVel);
	// 重力加速度
	json.AddValue<Vector3>("Acceleration", &dustCloudData_.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &dustCloudData_.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &dustCloudData_.maxElapseTime);

	// パーティクルの数
	json.AddValue<int>("Count", &dustCloudData_.count);
	json.EndGroup();
#pragma endregion
}