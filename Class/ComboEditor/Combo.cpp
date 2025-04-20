#include "Combo.h"

Combo::~Combo()
{
	// 派生コンボ配列内の要素削除
	for (Combo* c : childs_) {
		delete c;
	}
	// 配列の要素クリア
	childs_.clear();
}

void Combo::Init(const std::string& name)
{
	// 名称設定
	name_ = name;
	strncpy_s(imGuiName_, sizeof(imGuiName_), name_.c_str(), _TRUNCATE);

	// 攻撃関係変数のリセット
	attackStartTime_ = 0.0f;	// 開始時間
	attackEndTime_ = 0.0f;		// 終了時間
	isAttackActive_ = false;	// 攻撃判定

	// 硬直関係変数のリセット
	stifnessTime_ = 0.0f;		// 硬直時間
	isStifness_ = true;			// フラグ

	// コンボ受付関係変数のリセット
	receptTime_ = 0.0f;	// 受付時間
	isRecept_ = false;	// 受付可能フラグ
}

void Combo::Init(const std::string& fileName, const std::string& comboName)
{
	// Todo : ファイル読み込み作る
	fileName, comboName;

	// 初期化 Todo : 初期化の際に読み込んだコンボの名称を使用する
	Init("Combo");

	// 攻撃判定用タイマー開始
	attackDecisionTimer_.Start(attackStartTime_);	// 攻撃判定
	stifnessTimer_.Start(stifnessTime_);			// 硬直時間
	receptTimer_.Start(receptTime_);				// 受付時間
}

void Combo::Update()
{
	// 攻撃判定用のタイマーが動作している場合のみ更新を行う
	if (attackDecisionTimer_.GetIsActive()) {
		// 攻撃判定に関する更新
		AttackActiveUpdate();
	}

	// 硬直時間タイマーが動作している場合のみ更新を行う
	if (stifnessTimer_.GetIsActive()) {
		// 硬直に関する更新
		stifnessTimer_.Update();
	}

	// コンボ受付判定用のタイマーが動作している場合のみ更新を行う
	if (receptTimer_.GetIsActive()) {
		// コンボ受付時間に関する更新
		ReceptTimeUpdate();
	}
	else {
		// コンボ受付処理に関する更新
		ReceptUpdate();
	}
}

void Combo::NodeMenu(int& id, int& buttonID, Combo*& combo)
{
	// ボタンが選択されている場合色を変更する
	if (imGuiSelected_) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0, 255, 0, 1.0f));
	}

	// ラジオボタンの表示
	if (ImGui::RadioButton(name_.c_str(), &id, buttonID)) {
		// 編集対象のコンボとして自身を登録する
		combo = this;
	}

	// 変更した色を元に戻す必要があるためここで戻す
	if (imGuiSelected_) {
		ImGui::PopStyleColor(4);
	}

	// 選択フラグリセット
	imGuiSelected_ = false;

	// ボタンID加算
	buttonID++;

	// 子のコンボ要素が無ければこれ以降の処理は無視する
	if (childs_.empty()) { return; }

	// ずらす
	ImGui::Indent();

	// 子供のノードメニューを展開する
	for (Combo* c : childs_) {
		c->NodeMenu(id, buttonID, combo);
	}

	ImGui::Unindent();
}

void Combo::DebugGUI()
{
	// この処理に入っている場合選択されている
	imGuiSelected_ = true;

	// コンボ編集メニュー
	ImGui::Text("Combo Edit Menu");

	// コンボ名称の編集
	ImGui::InputText("Name", imGuiName_, sizeof(imGuiName_));
	name_ = imGuiName_;

	// 再生されるアニメーションの設定
	ImGui::InputText("AnimName", imGuiAnimName_, sizeof(imGuiAnimName_));
	animName_ = imGuiAnimName_;

	// 攻撃判定関係の設定
	if (ImGui::TreeNode("Attack Edit")) {
		// 開始時間
		ImGui::DragFloat("AttackStartTime", &attackStartTime_, 0.01f, 0.0f);
		// 終了時間
		ImGui::DragFloat("AttackEndTime", &attackEndTime_, 0.01f, 0.0f);
		ImGui::TreePop();
	}

	// 硬直時間関係の設定
	if (ImGui::TreeNode("Stifness Time Edit")) {
		// 開始時間
		ImGui::DragFloat("StifnessTime", &stifnessTime_, 0.01f, 0.0f);
		ImGui::TreePop();
	}

	// 受付時間関係の設定
	if (ImGui::TreeNode("Recept Time Edit")) {
		// 開始時間
		ImGui::DragFloat("ReceptTime", &receptTime_, 0.01f, 0.0f);
		ImGui::TreePop();
	}
}

bool Combo::GetConditions()
{
	// 条件配列の中身が無ければ条件を満たしているとする
	if (conditions_.size() == 0) { return true; }

	// 配列分ループする
	for (LWP::Utility::ICondition* c : conditions_) {
		// １つでも条件を達成していなければ早期リターン
		if (!c->CheckCondition()) { return false; }
	}

	// ここまで到達した場合条件を満たしているとする
	return true;
}

void Combo::CreateChild(const std::string& name)
{
	// 新規コンボの生成
	Combo* c = new Combo();
	// コンボの初期化
	c->Init(name);

	// 派生コンボ配列に追加
	childs_.push_back(std::move(c));
}

void Combo::AttackActiveUpdate()
{
	// タイマーの更新
	attackDecisionTimer_.Update();

	// 攻撃判定タイマーが終了していれば
	if (attackDecisionTimer_.GetIsFinish()) {
		// 攻撃判定が有効になっていなければ
		if (!isAttackActive_) {
			// 攻撃判定有効
			isAttackActive_ = true;
			// 攻撃判定終了秒数でタイマー開始
			attackDecisionTimer_.Start(attackEndTime_);
		}
		else {
			// 攻撃判定無効
			isAttackActive_ = false;
			// 攻撃判定タイマーを非アクティブに
			attackDecisionTimer_.SetIsActive(false);
		}
	}
}

void Combo::StifnessTimeUpdate()
{
	// タイマーの更新
	stifnessTimer_.Update();

	// 硬直時間タイマーが終了している場合
	if (stifnessTimer_.GetIsFinish()) {
		// 硬直終了
		isStifness_ = false;
		// 硬直時間タイマーを非アクティブに
		stifnessTimer_.SetIsActive(false);
	}
}

void Combo::ReceptTimeUpdate()
{
	// タイマーの更新
	receptTimer_.Update();

	// 受付時間タイマーが終了している場合
	if (receptTimer_.GetIsFinish()) {
		// 次のコンボへ移る操作を受け付ける
		isRecept_ = true;
		// 受付時間タイマーを非アクティブに
		receptTimer_.SetIsActive(false);
	}
}

Combo* Combo::ReceptUpdate()
{
	// 派生コンボ内に遷移できるコンボがあった場合そのコンボを返す
	for (Combo* c : childs_) {
		if (c->GetConditions()) { return c; }
	}

	// 遷移できるコンボがない場合nullptrを返す
	return nullptr;
}
