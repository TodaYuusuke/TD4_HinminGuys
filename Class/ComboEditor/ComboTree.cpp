#include "ComboTree.h"

ComboTree::~ComboTree()
{
	// 派生コンボ配列内の要素削除
	for (Combo* c : baseCombos_) {
		delete c;
	}
	// 配列の要素クリア
	baseCombos_.clear();
}

void ComboTree::Init()
{	
	// 新規大元コンボを追加する
	Combo* c = new Combo();
	c->Init("Combo");
	editingCombo_ = c;
	baseCombos_.push_back(std::move(c));
}

void ComboTree::Update()
{

}

void ComboTree::DebugGUI()
{
	// ImGui開始
	ImGui::Begin("ComboManager", nullptr, ImGuiWindowFlags_MenuBar);

	// メニューバーの開始
	if (ImGui::BeginMenuBar()) {

		ImGui::EndMenuBar();
	}

	// 全オブジェクトのImGuiを描画
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 300), ImGuiWindowFlags_NoTitleBar);
	
	// ノードメニューを表示する
	int editID = 0;
	Combo* change = nullptr;

	ImGui::SetWindowFontScale(1.35f);

	// 大元のコンボ配列分ループする
	for (Combo* c : baseCombos_) {
		c->NodeMenu(selectedID_, editID, change);

		// もし編集対象が変更されている場合
		if (change != nullptr) {
			editingCombo_ = change;
		}
	}

	ImGui::SetWindowFontScale(1.0f);

	ImGui::EndChild();

	// 編集中コンボの情報表示
	editingCombo_->DebugGUI();

	// 編集中コンボに対して派生コンボを生成するボタンを追加する
	if (ImGui::Button("Create Child")) {
		CreateChild();
	}

	ImGui::End();
}

void ComboTree::CreateChild()
{
	// 選択されたコンボに対して派生コンボを追加する
	editingCombo_->CreateChild("Child");
}
