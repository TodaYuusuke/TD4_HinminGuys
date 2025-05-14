#include "ComboTree.h"

using namespace LWP::Utility::Condition;
using namespace LWP;

ComboTree::~ComboTree()
{
	
}

void ComboTree::Init(const std::string& fileName, LWP::Resource::SkinningModel* model, LWP::Resource::Animation* anim)
{
	#ifdef _DEBUG
	// デバッグ時、デフォルトで編集モード有効
	enableEditMode_ = true;
	#endif // _DEBUG

	// モデル、アニメーションを取得
	animModel_ = model;
	anim_ = anim;

	// 無操作状態のコンボの初期化
	rootCombo_.Init("Neutral");
	editingCombo_ = &rootCombo_;
	rootCombo_.SetIsRoot(true);

	// コンボのロード
	fileName_ = fileName;
	LoadCombo(fileName_);

	// 現在コンボに無操作状態のコンボを設定する
	nowCombo_ = &rootCombo_;
	// 現在コンボのリセット
	nowCombo_->Init();
	// 現在コンボのスタート
	nowCombo_->Start(anim_);
}

void ComboTree::Update()
{
	// もし編集モードが有効であれば更新処理をスキップ
	if (enableEditMode_) { return; }

	// 現在コンボの更新
	nowCombo_->Update(animModel_, anim_);

	// コンボの受付処理
	nextCombo_ = nowCombo_->ReceptUpdate();

	// 次のコンボが存在する、かつ硬直時間終了時
	if ((nextCombo_ != nullptr && nextCombo_->GetIsStifness())) {
		// 現在のコンボを初期化して次のコンボへ
		nowCombo_->Init();
		nowCombo_ = std::move(nextCombo_);
		nowCombo_->Start(anim_);
		return;
	}

	// 次のコンボが存在しない、かつコンボ受付が終了している場合
	if (nextCombo_ == nullptr && !nowCombo_->GetIsRecept()) {
		nowCombo_->Init();
		nowCombo_ = &rootCombo_;
		nowCombo_->Start(anim_);
	}
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

	// 編集モードが有効、かつコンボが選択されている場合
	if (enableEditMode_ && editingCombo_ != nullptr) {
		editingCombo_->DebugGUI();

		// アニメーション簡易設定メニュー
		AnimNameEasySetter();

		// 派生コンボの生成処理
		CreateChildMenu();

		// 開始条件の生成処理
		CreateConditionMenu();

		// コンボの削除処理
		DeletePopUp();

		// 実行モードへ移るボタン処理
		ImGui::SeparatorText("Change RunningMode");
		// ボタンを押した際に実行モードへ移る
		if (ImGui::Button("Change")) {
			enableEditMode_ = false;
		}

	}
	else { // 編集モードが有効でない場合
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		// フォントサイズの調整
		ImGui::SetWindowFontScale(1.5f);

		// 現在が実行モードであることを伝える
		ImGui::Text("Currently Running Mode!");
		ImGui::Text("Please Change EditMode!");
		// 編集モードを有効にするボタンを表示させる
		if (ImGui::Button("Enable EditMode")) {
			// 編集コンボを無操作状態コンボにする
			editingCombo_ = &rootCombo_;

			// 現在のコンボを強制的に初期化
			nowCombo_->Init();
			// 現在コンボを強制的に無操作状態のコンボに
			nowCombo_ = &rootCombo_;

			// 押されたら編集モードを有効に
			enableEditMode_ = true;
		}

		// フォントサイズのリセット
		ImGui::SetWindowFontScale(1.0f);

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}

	ImGui::End();
}

bool ComboTree::GetIsStiffness()
{
	// 現在コンボに何も入っていなければfalseに
	if (nowCombo_ == nullptr) { return false; }

	// 硬直状態を返す
	return nowCombo_->GetIsStifness();
}

void ComboTree::FileMenu()
{
	// ファイル関係メニューの表示
	if (ImGui::BeginMenu("File")) {

		// 作成したツリーの保存を行う
		if (ImGui::MenuItem("Save")) {
			// コンボの保存
			SaveCombo();
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

	rootCombo_.NodeMenu(selectedID_, editID, change);

	// もし編集対象が変更されている場合
	if (change != nullptr) {
		editingCombo_ = change;

		// 生成する派生コンボの名称もそのコンボ名称に変更しておく
		std::string editComboName = editingCombo_->GetName();
		// 末尾に番号があれば取り外す
		while (isdigit(editComboName.at(editComboName.size() - 1)))
		{
			// 末尾の文字を削除
			editComboName.pop_back();
		}

		imGuiChildComboName_ = editComboName;
	}

	ImGui::SetWindowFontScale(1.0f);
}

void ComboTree::SaveCombo()
{
	jsonIO_.Init(fileName_);

	// 全コンボの保存
	rootCombo_.AddValue(jsonIO_);
	jsonIO_.Save();
}

void ComboTree::LoadCombo(const std::string& fileName)
{
	// 初期化
	jsonIO_.Init(fileName);
	jsonIO_.CheckJsonFile();

	// グループ名の取得
	Utility::NestedList nameList = Utility::JsonIO::LoadGroupNames(fileName);

	// グループ名リストが空の場合早期リターン
	if (nameList.empty()) {
		return;
	}

	// 同名の空のコンボ配列を作成して読み込めるようにする
	auto lamda = [](auto self, Utility::NestedList& list, Combo& c) -> void {
		for (auto itr = list.begin(); itr != list.end(); ++itr) {
			// 名前を' | 'で分割する
			std::vector<std::string> splitName = Utility::Split(itr->name, '|');

			// 最初の要素に開始条件という文章が含まれているなら
			if (splitName[0] == "StartConditions") {
				// その後に続く文章で何の条件か判断し、追加する
				if (splitName[1] == "Button") {
					// 新しい開始条件の生成
					Utility::ButtonCondition* condition =
						new Utility::ButtonCondition(TRIGGER, Controller::X, KeyBoard::J);

					// 追加
					c.AddCondition(condition);
				}

				// 次のループへ
				continue;
			}

			// 再帰的に空のコンボを生成する
			self(self, itr->list, c.CreateChild(itr->name));
		}
		};
	// ラムダ式を実行
	lamda(lamda, nameList[0].list, rootCombo_);

	// 値のロード
	rootCombo_.AddValue(jsonIO_);
	jsonIO_.Load();

	// ロード後、派生優先度によって配列を並び替える
	rootCombo_.SortByPriorityAll();
}

int ComboTree::GetSameNameCount(const std::string& name)
{
	// 同名コンボのカウント用
	int SameNameCount = 0;

	// コンボ名称取得、末尾に番号があれば取り外す
	std::string comboName = name;
	while (isdigit(comboName.at(comboName.size() - 1)))
	{
		// 末尾の文字を削除
		comboName.pop_back();
	}

	// 同名コンボを全て探す
	rootCombo_.SameNameCount(comboName, SameNameCount);

	// 結果を返す
	return SameNameCount;
}

void ComboTree::AnimNameEasySetter()
{
	ImGui::SeparatorText("AnimNameEasySetter");
	// アニメーションがセットされていなければここで早期リターン
	if (anim_ == nullptr) {
		ImGui::Text("Animation Class Isn't Set");
		ImGui::TreePop();
		return;
	}

	// アニメーションからアニメーション名を取得
	std::vector<std::string>animNames = anim_->GetAnimationNames();

	// 取得したアニメーションが無ければ早期リターン
	if (animNames.empty()) {
		ImGui::Text("No Animations!");
		ImGui::TreePop();
		return;
	}

	// アニメーション名をメニューで表示
	if (ImGui::BeginMenu("Please Select Names")) {
		for (std::string& name : animNames) {
			if (name == "") { continue; }

			// 選択すると該当するアニメーション名に切り替える
			if (ImGui::MenuItem(name.c_str())) {
				editingCombo_->SetAnimName(name);
			}
		}
		ImGui::EndMenu();
	}

	ImGui::NewLine();
}

void ComboTree::CreateChildMenu()
{
	// 派生コンボの生成に関する処理
	ImGui::SeparatorText("CreateChild");

	// 生成する派生コンボの名称設定
	Base::ImGuiManager::InputText("Child Name", imGuiChildComboName_);

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
	}

	ImGui::NewLine();
}

void ComboTree::CreateConditionMenu()
{
	// 派生コンボの生成に関する処理
	ImGui::SeparatorText("Create StartCondition");
	// ボタン変更メニュー
	if (ImGui::BeginMenu("Select Condition")) {
		if (ImGui::BeginMenu("Input")) {
			InputMenu();
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	// 改行
	ImGui::NewLine();
}

void ComboTree::InputMenu()
{
	if (ImGui::MenuItem("Button")) {
		// 新しい開始条件の生成
		Utility::ButtonCondition* c = 
			new Utility::ButtonCondition(TRIGGER, Controller::X, KeyBoard::J);

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
