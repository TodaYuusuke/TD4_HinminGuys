#include "SlashEffector.h"

using namespace LWP;

SlashEffector::SlashEffector(const std::string& tex, const LWP::Math::Vector2& size, const int maxFrame)
{
	// テクスチャ名の取得
	texName_ = tex;
	// 1フレームごとのテクスチャサイズの取得
	frameSize_ = size;
	// 最大フレーム数の取得
	maxFrame_ = maxFrame;
	
	// エフェクト配列のクリア
	effects_.clear();
}

SlashEffector::~SlashEffector()
{
	// エフェクトとプレーンの明示的な削除
	effects_.remove_if([](SlashEffectData* e) {
		delete e->plane;
		return true;
	});
}

void SlashEffector::Update()
{
	// 終了フラグのたったエフェクトを削除
	effects_.remove_if([](SlashEffectData* e) {
		if (e->isEnd) {
			delete e->plane;
			return true;
		}
		return false;
	});

	// 配列の更新
	for (SlashEffectData* e : effects_) {
		// タイマーが終了状態であればエフェクトを終了状態に
		if (e->aliveTimer.GetIsFinish()) { e->isEnd = true; }

		// インデックスを調整
		e->plane->index = e->frame;
		// タイマー更新
		e->aliveTimer.Update();
		// タイマーの進捗を元にフレームを更新
		e->frame = static_cast<int>(maxFrame_ * e->aliveTimer.GetProgress());
	}
}

void SlashEffector::Create(const LWP::Math::Vector3& pos, const LWP::Math::Quaternion& rotate, const LWP::Math::Vector3& scale, const float playTime)
{
	// エフェクトデータ生成
	SlashEffectData* effect = new SlashEffectData();

	// テクスチャ読み込み
	effect->plane = new LWP::Primitive::SequenceSurface();
	effect->plane->LoadTexture(texName_);

	// 座標設定
	if (parentTF != nullptr) {
		// 親子付け対象がいる場合生成する平面を親子付けする
		effect->plane->worldTF.Parent(parentTF);
	}
	effect->plane->worldTF.translation = pos;
	effect->plane->worldTF.rotation = rotate;
	effect->plane->worldTF.scale = scale;
	// サイズ設定
	effect->plane->SetSplitSize(frameSize_);
	// 生存時間タイマー開始
	effect->aliveTimer.Start(playTime);

	// 生成したエフェクトデータを配列に追加する
	effects_.emplace_back(std::move(effect));
}

void SlashEffector::DebugGUI()
{
	// デバッグ用生成処理
	ImGui::SeparatorText("Debug Generate");

	// 座標、回転角設定
	ImGui::DragFloat3("Position", &imGuiPos_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &imGuiRotate_.x, 0.1f);
	ImGui::DragFloat3("Scale", &imGuiScale_.x, 0.1f, 0.001f);

	// 生存時間の設定
	ImGui::DragFloat("AliveTime", &imGuiAliveTime_, 0.01f, 0.0f);

	// 読み込みテクスチャ名称の設定
	Base::ImGuiManager::InputText("TextureName", imGuiLoadTexName_);

	// 生成
	if (ImGui::Button("Generate")) {
		texName_ = imGuiLoadTexName_;
		Create(imGuiPos_, imGuiRotate_, imGuiScale_, imGuiAliveTime_);
	}
}
