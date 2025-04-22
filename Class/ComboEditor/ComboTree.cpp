#include "ComboTree.h"

using namespace LWP::Utility::Condition;

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
		// ファイルメニュー
		FileMenu();
		ImGui::EndMenuBar();
	}

	// ノード画面を格納するためのウィンドウを作る
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 300), ImGuiWindowFlags_NoTitleBar);
	
	// ノードメニューの展開
	NodeMenu();

	ImGui::EndChild();

	// 編集中コンボのGUI情報表示
	if (editingCombo_ != nullptr) {
		editingCombo_->DebugGUI();

		// 派生コンボの生成処理
		CreateChildMenu();

		// 開始条件の生成処理
		CreateConditionMenu();

		// コンボの削除処理
		DeletePopUp();
	}

	ImGui::End();
}

void ComboTree::FileMenu()
{
	// ファイル関係メニューの表示
	if (ImGui::BeginMenu("File")) {

		// 作成したツリーの保存を行う
		if (ImGui::MenuItem("Save")) {

		}

		// ツリーの再読み込み
		if (ImGui::MenuItem("Load")) {

		}

		ImGui::EndMenu();
	}
}

void ComboTree::NodeMenu()
{
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

			// 生成する派生コンボの名称もそのコンボ名称に変更しておく
			strncpy_s(imGuiChildComboName_, sizeof(imGuiChildComboName_), editingCombo_->GetName().c_str(), _TRUNCATE);
		}
	}

	ImGui::SetWindowFontScale(1.0f);
}

int ComboTree::GetSameNameCount(const std::string& name)
{
	// 同名コンボのカウント用
	int SameNameCount = 0;

	// 大元コンボ分ループ
	for (Combo* c : baseCombos_) {
		// 同名コンボを全て探す
		c->SameNameCount(name, SameNameCount);
	}

	// 結果を返す
	return SameNameCount;
}

void ComboTree::CreateChildMenu()
{
	// 派生コンボの生成に関する処理
	ImGui::SeparatorText("CreateChild");

	// 生成する派生コンボの名称設定
	ImGui::InputText("Child Name", imGuiChildComboName_, sizeof(imGuiChildComboName_));

	ImGui::SameLine();

	// 派生コンボの生成処理
	if (ImGui::Button("Create")) {
		// 生成するコンボ名称に被りがないか確認する
		std::string childComboName = imGuiChildComboName_;
		int sameCount = GetSameNameCount(childComboName);

		// 同名コンボが複数個確認された場合名称を変更する
		if (sameCount > 0) { childComboName += std::to_string(sameCount + 1); }

		// 編集対象に派生コンボを追加する
		editingCombo_->CreateChild(childComboName);

		// 生成時点で名称をデフォルト設定に戻す
		strncpy_s(imGuiChildComboName_, sizeof(imGuiChildComboName_), "Child", _TRUNCATE);
	}

	ImGui::NewLine();
}

void ComboTree::CreateConditionMenu()
{
	// 派生コンボの生成に関する処理
	ImGui::SeparatorText("Create StartCondition");
	// ボタン変更メニュー
	if (ImGui::BeginMenu("Select Condition")) {
		if (ImGui::BeginMenu("Contoroller Input")) {
			ControllerInputMenu();
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	// 改行
	ImGui::NewLine();
}

void ComboTree::ControllerInputMenu()
{
	if (ImGui::MenuItem("Button")) {
		// 新しい開始条件の生成
		LWP::Utility::ControllerButtonCondition* c = 
			new LWP::Utility::ControllerButtonCondition(TRIGGER, Controller::X);

		// 追加
		editingCombo_->AddCondition(c);
	}
}

void ComboTree::DeletePopUp()
{
	// コンボ削除に関する処理
	ImGui::SeparatorText("DeleteMenu");

	// 削除処理
	if (ImGui::Button("Delete Combo")) {
		// ポップアップ表示
		imGuiIsOpenPopUp_ = true;
		ImGui::OpenPopup("Delete Combo");
	}

	// ポップアップが表示されていない場合早期リターン
	if (!imGuiIsOpenPopUp_) {
		return;
	}

	// 中央に表示する（オプション）
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// ウィンドウのスケール調整
	ImGui::SetWindowFontScale(1.5f);

	// ポップアップの起動
	if (ImGui::BeginPopupModal("Delete Combo", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		// コンボを削除するか否か確認する
		ImGui::Text("Do you want to remove this combo?");

		// はい
		if (ImGui::Button("Yes")) {
			// 派生コンボの削除
			editingCombo_->DeleteThis();

			imGuiIsOpenPopUp_ = false;
			// 開いたウィンドウを閉じる
			ImGui::CloseCurrentPopup();
		}

		// 改行しない
		ImGui::SameLine();

		// いいえ
		if (ImGui::Button("No")) {
			imGuiIsOpenPopUp_ = false;
			// 開いたウィンドウを閉じる
			ImGui::CloseCurrentPopup();
		}

		// ポップアップ終了
		ImGui::EndPopup();
	}

	// ウィンドウのスケール調整
	ImGui::SetWindowFontScale(1.0f);
}
