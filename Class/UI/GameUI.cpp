#include "GameUI.h"

using namespace LWP::Utility;

std::array<const char*, int(LWP::Utility::Easing::Type::EasingCount)> GameUI::easingName = {
	"Liner", "InQuint", "OutQuint", "InOutQuint", "InCirc", "OutCirc", "InOutCirc", 
	"InCubic", "OutCubic", "InOutCubic", "InBack", "OutBack", "InOutBack",
	"InBounce", "OutBounce", "InOutBounce", "InElastic", "OutElastic", "InOutElastic",
	"InQuart", "OutQuart", "InOutQuart", "InExpo", "OutExpo", "InOutExpo"
};

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

void GameUI::Initialize(const std::string& fileName, const std::string& UIName) {

	sprite_.LoadTexture(fileName);
	sprite_.anchorPoint = { 0.5f,0.5f };
	sprite_.Init();
	name_ = UIName;

	json_.Init(name_ + ".json");

	//非アクティブ状態のデータ
	json_.BeginGroup("IdleData")
		.AddValue<LWP::Math::Vector3>("StartTranslation", &idleData.startTranslation)
		.AddValue<LWP::Math::Vector3>("EndTranslation", &idleData.endTranslation)
		.AddValue<LWP::Math::Vector3>("StartScale", &idleData.startScale)
		.AddValue<LWP::Math::Vector3>("EndScale", &idleData.endScale)
		.AddValue<LWP::Math::Vector3>("StartColor", &idleData.startColor)
		.AddValue<LWP::Math::Vector3>("EndColor", &idleData.endColor)
		.AddValue<int>("EasingType", reinterpret_cast<int*>(&idleData.easingType))
		.AddValue<float>("MoveTime", &idleData.moveTime)
		.AddValue<bool>("IsLoop", &idleData.isLoop)
		.EndGroup()
		// アクティブ状態のデータ
		.BeginGroup("ActiveData")
		.AddValue<LWP::Math::Vector3>("StartTranslation", &activeData.startTranslation)
		.AddValue<LWP::Math::Vector3>("EndTranslation", &activeData.endTranslation)
		.AddValue<LWP::Math::Vector3>("StartScale", &activeData.startScale)
		.AddValue<LWP::Math::Vector3>("EndScale", &activeData.endScale)
		.AddValue<LWP::Math::Vector3>("StartColor", &activeData.startColor)
		.AddValue<LWP::Math::Vector3>("EndColor", &activeData.endColor)
		.AddValue<int>("EasingType", reinterpret_cast<int*>(&activeData.easingType))
		.AddValue<float>("MoveTime", &activeData.moveTime)
		.AddValue<bool>("IsLoop", &activeData.isLoop)
		.EndGroup()
		.CheckJsonFile();

}

void GameUI::Update() {

	if (isActive) {
		EasingUpdate(activeData);
	}
	else {
		EasingUpdate(idleData);
	}

}

void GameUI::DebugGUI() {

	if (ImGui::TreeNode(name_.c_str())) {
		// 保存する値
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			ImGui::TreePop();
		}
		
		//非アクティブ状態のデバッグ
		if (ImGui::TreeNode("IdleData")) {
			
			ImGui::DragFloat3("StartTranslation", &idleData.startTranslation.x);
			ImGui::DragFloat3("EndTranslation", &idleData.endTranslation.x);
			ImGui::DragFloat3("StartScale", &idleData.startScale.x, 0.1f);
			ImGui::DragFloat3("EndScale", &idleData.endScale.x, 0.1f);
			ImGui::DragFloat3("StartColor", &idleData.startColor.x, 1.0f, 0.0f, 255.0f);
			ImGui::DragFloat3("EndColor", &idleData.endColor.x, 1.0f, 0.0f, 255.0f);
			ImGui::DragFloat("MoveTime", &idleData.moveTime, 0.1f);

			if (Easing::SelectTypeDebugGUI(&idleData.easingType)) {

			}

			ImGui::Checkbox("IsLoop", &idleData.isLoop);

			ImGui::TreePop();
		}

		//アクティブ状態のデバッグ
		if (ImGui::TreeNode("ActiveData")) {
			
			ImGui::DragFloat3("StartTranslation", &activeData.startTranslation.x);
			ImGui::DragFloat3("EndTranslation", &activeData.endTranslation.x);
			ImGui::DragFloat3("StartScale", &activeData.startScale.x, 0.1f);
			ImGui::DragFloat3("EndScale", &activeData.endScale.x, 0.1f);
			ImGui::DragFloat3("StartColor", &activeData.startColor.x, 1.0f, 0.0f,255.0f);
			ImGui::DragFloat3("EndColor", &activeData.endColor.x, 1.0f, 0.0f, 255.0f);
			ImGui::DragFloat("MoveTime", &activeData.moveTime, 0.1f);

			if (Easing::SelectTypeDebugGUI(&activeData.easingType)) {

			}

			ImGui::Checkbox("IsLoop", &activeData.isLoop);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

}

void GameUI::EasingUpdate(UIParameter& parameter)
{

	//時間を加算(リバース状態では減算)
	if (parameter.isReverse) {

		if (parameter.currentTime > 0.0f) {
			parameter.currentTime -= 1.0f * LWP::Info::GetDeltaTimeF();
		}

	}
	else {

		if (parameter.currentTime < parameter.moveTime) {
			parameter.currentTime += 1.0f * LWP::Info::GetDeltaTimeF();
		}

	}

	

	//範囲時間を超えたら
	if (not parameter.isReverse and parameter.currentTime >= parameter.moveTime) {

		//ループなら0に戻す
		if (parameter.isLoop) {
			parameter.currentTime = parameter.moveTime;
			parameter.isReverse = true;
		}
		else {
			parameter.currentTime = parameter.moveTime;
		}
		
	}

	if (parameter.isReverse and parameter.currentTime <= 0.0f) {

		//ループなら0に戻す
		if (parameter.isLoop) {
			parameter.currentTime = 0.0f;
			parameter.isReverse = false;
		}
		else {
			parameter.currentTime = 0.0f;
		}

	}

	//イージングのT
	float t = 0.0f;

	//0除算を避ける
	if (parameter.moveTime != 0.0f) {
		//セットしているイージング関数で処理
		t = Easing::CallFunction(parameter.easingType, parameter.currentTime / parameter.moveTime);
	}

	//トランスフォーム更新
	sprite_.worldTF.translation = LWP::Utility::Interpolation::Lerp(parameter.startTranslation, parameter.endTranslation, t);
	sprite_.worldTF.scale = LWP::Utility::Interpolation::Lerp(parameter.startScale, parameter.endScale, t);

	//色更新
	LWP::Math::Vector3 color = LWP::Math::Vector3::Lerp(parameter.startColor, parameter.endColor, t);
	sprite_.material.color.R = color.x;
	sprite_.material.color.G = color.y;
	sprite_.material.color.B = color.z;

}

float GameUI::Easing(LWP::Utility::Easing::Type type, float x)
{

	//タイプに応じて関数をセット
	switch (type)
	{
	case LWP::Utility::Easing::Type::Liner:
		return LWP::Utility::Easing::Liner(x);
		break;
	case LWP::Utility::Easing::Type::InQuint:
		return LWP::Utility::Easing::InQuint(x);
		break;
	case LWP::Utility::Easing::Type::OutQuint:
		return LWP::Utility::Easing::OutQuint(x);
		break;
	case LWP::Utility::Easing::Type::InOutQuint:
		return LWP::Utility::Easing::InOutQuint(x);
		break;
	case LWP::Utility::Easing::Type::InCirc:
		return LWP::Utility::Easing::InCirc(x);
		break;
	case LWP::Utility::Easing::Type::OutCirc:
		return LWP::Utility::Easing::OutCirc(x);
		break;
	case LWP::Utility::Easing::Type::InOutCirc:
		return LWP::Utility::Easing::InOutCirc(x);
		break;
	case LWP::Utility::Easing::Type::InCubic:
		return LWP::Utility::Easing::InCubic(x);
		break;
	case LWP::Utility::Easing::Type::OutCubic:
		return LWP::Utility::Easing::OutCubic(x);
		break;
	case LWP::Utility::Easing::Type::InOutCubic:
		return LWP::Utility::Easing::InOutCubic(x);
		break;
	case LWP::Utility::Easing::Type::InBack:
		return LWP::Utility::Easing::InBack(x);
		break;
	case LWP::Utility::Easing::Type::OutBack:
		return LWP::Utility::Easing::OutBack(x);
		break;
	case LWP::Utility::Easing::Type::InOutBack:
		return LWP::Utility::Easing::InOutBack(x);
		break;
	case LWP::Utility::Easing::Type::InBounce:
		return LWP::Utility::Easing::InBounce(x);
		break;
	case LWP::Utility::Easing::Type::OutBounce:
		return LWP::Utility::Easing::OutBounce(x);
		break;
	case LWP::Utility::Easing::Type::InOutBounce:
		return LWP::Utility::Easing::InOutBounce(x);
		break;
	case LWP::Utility::Easing::Type::InElastic:
		return LWP::Utility::Easing::InElastic(x);
		break;
	case LWP::Utility::Easing::Type::OutElastic:
		return LWP::Utility::Easing::OutElastic(x);
		break;
	case LWP::Utility::Easing::Type::InOutElastic:
		return LWP::Utility::Easing::InOutElastic(x);
		break;
	case LWP::Utility::Easing::Type::InQuart:
		return LWP::Utility::Easing::InQuart(x);
		break;
	case LWP::Utility::Easing::Type::OutQuart:
		return LWP::Utility::Easing::OutQuart(x);
		break;
	case LWP::Utility::Easing::Type::InOutQuart:
		return LWP::Utility::Easing::InOutQuart(x);
		break;
	case LWP::Utility::Easing::Type::InExpo:
		return LWP::Utility::Easing::InExpo(x);
		break;
	case LWP::Utility::Easing::Type::OutExpo:
		return LWP::Utility::Easing::OutExpo(x);
		break;
	case LWP::Utility::Easing::Type::InOutExpo:
		return LWP::Utility::Easing::InOutExpo(x);
		break;
	default:
		break;
	}

	return LWP::Utility::Easing::Liner(x);

}
