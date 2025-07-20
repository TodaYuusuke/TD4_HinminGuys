#include "Chain.h"
#include "../../../../../Components/HitStopController.h"

using namespace LWP;
using namespace LWP::Math;

Chain::Chain() {
	// サンプルのばね
	sampleSpring_ = std::make_unique<ChainNode>(0, 0);
	sampleSpring_->Initialize();
	//sampleSpring_->CreateJsonData();

	CreateJsonData();
	segmentLength = sampleSpring_->GetSpring().restLength;
}

void Chain::Initialize() {
	pointMassList_.clear();
	chainNodeList_.clear();

	segmentLength = sampleSpring_->GetSpring().restLength;

	for (int i = 0; i < segmentCount; ++i) {
		// 質点の設定
		PointMass pointMass = {
			.velocity{0, 0, 0},
			.acceleration = defaultAcceleration,
			.force{0, 0, 0},
			.isFixed = (i == 0 || i == segmentCount - 1) // 最初の1つを固定
		};
		pointMass.model.LoadCube();
		pointMass.model.worldTF.scale = { 0.05f,0.05f,0.05f };  // 大きさ
		// 始点
		if (pointMass.isFixed && i == 0) {
			pointMass.model.worldTF.translation = startPos_;  // 位置
		}
		// 終点
		else {
			Vector3 dir = (endPos_ - startPos_).Normalize();
			pointMass.model.worldTF.translation = startPos_ - dir * i * segmentLength;
		}
		pointMassList_.push_back(pointMass);

		// 鎖ノード作成
		if (i > 0) {
			// ばねの設定を作成
			Spring spring = sampleSpring_->GetSpring();
			spring.p1 = i - 1;
			spring.p2 = i;

			// 鎖ノード生成
			ChainNode* chainNode = new ChainNode(spring);
			chainNode->SetSpring(spring);
			// 質点リストのアドレスを設定
			chainNode->SetPointMass(&pointMassList_);
			chainNodeList_.push_back(chainNode);
		}
	}
}

void Chain::Update() {
	if (!isActive_) { return; }

	// 質点の始点終点の更新
	pointMassList_[0].model.worldTF.translation = startPos_;
	int size = (int)pointMassList_.size() - 1;
	pointMassList_[size].model.worldTF.translation = endPos_;

	// 質点の座標を算出
	for (ChainNode* chainNode : chainNodeList_) {
		chainNode->Update();
		// 速度初期化
		pointMassList_[chainNode->GetSpring().p1].velocity = { 0.0f,0.0f,0.0f };
		pointMassList_[chainNode->GetSpring().p2].velocity = { 0.0f,0.0f,0.0f };
	}
	// 逆順に質点の座標を算出
	for (int i = (int)chainNodeList_.size() - 1; i >= 0; i--) {
		chainNodeList_[i]->InverseUpdate();
		// 速度初期化
		pointMassList_[chainNodeList_[i]->GetSpring().p1].velocity = { 0.0f,0.0f,0.0f };
		pointMassList_[chainNodeList_[i]->GetSpring().p2].velocity = { 0.0f,0.0f,0.0f };
	}

	for (PointMass& pointMass : pointMassList_) {
		if (pointMass.model.worldTF.translation.y <= 0.0f) {
			pointMass.model.worldTF.translation.y = std::clamp<float>(pointMass.model.worldTF.translation.y, 0.0f, 10000.0f);
		}
	}
}

void Chain::CreateJsonData() {
	json_.Init("ChainNode.json");
	// 鎖
	json_.BeginGroup("Chain");
	json_.AddValue<int>("Count", &segmentCount);
	json_.AddValue<Vector3>("Acceleration", &defaultAcceleration);
	json_.EndGroup();
	// ばね
	sampleSpring_->SetJsonData(json_);

	json_.CheckJsonFile();
}

void Chain::DebugGui() {
	json_.DebugGUI();
	// 鎖の配置間隔を自然長と同期させる
	segmentLength = sampleSpring_->GetSpring().restLength;

	//ImGui::DragFloat3("Base:Translation", &pointMassList_[0].model.worldTF.translation.x, 0.1f);
	ImGui::DragFloat3("Acceleration", &defaultAcceleration.x, 0.1f, 0, 0, "%.8f");

	// 設定を反映させる
	if (ImGui::Button("Settings completed")) {
		Initialize();
	}
}

void Chain::Reset() {
	pointMassList_.clear();
	chainNodeList_.clear();
}
