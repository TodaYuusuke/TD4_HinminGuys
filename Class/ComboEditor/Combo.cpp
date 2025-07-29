#include "Combo.h"

using namespace LWP;

Combo::~Combo()
{
	// 遷移条件配列内の要素削除
	for (Utility::ICondition* c : conditions_) {
		delete c;
	}
	// 配列の要素クリア
	conditions_.clear();

	// 派生コンボ配列内の要素削除
	for (Combo* c : childs_) {
		delete c;
	}
	// 配列の要素クリア
	childs_.clear();
}

void Combo::Init()
{
	// 攻撃関係変数のリセット
	isAttackActive_ = false;
	isAttackAssistActive_ = false;

	// 硬直関係変数のリセット
	isStifness_ = false;

	// コンボ受付関係変数のリセット
	isRecept_ = false;
}

void Combo::Init(const std::string& name)
{
	// 初期化
	Init();

	// 名称設定
	name_ = name;
}

void Combo::Start(LWP::Resource::SkinningModel* model, LWP::Resource::Animation* anim, LWP::Object::Collision* collider, SlashEffector* effector)
{
	// 判定用タイマー開始
	attackDecisionTimer_.Start(attackStartTime_);		// 攻撃判定
	attackAssistTimer_.Start(attackAssistStartTime_);	// 攻撃アシスト判

	if (startSlashEffectTime_ > 0.0f) {  // 斬撃エフェクトの再生開始時間
		// 斬撃エフェクトの開始秒数が0秒以上の場合タイマーを開始
		slashEffectTimer_.Start(startSlashEffectTime_); 
	}
	else if(playSlashEffectTime_ > 0.0f){ // 0秒以下の場合
		// タイマーをアクティブ状態にしておく
		slashEffectTimer_.SetIsActive(true);
	}

	if (stifnessTime_ > 0.0f) { // 硬直時間
		// 硬直秒数が0秒以上の場合タイマーを開始する
		stifnessTimer_.Start(stifnessTime_);
		isStifness_ = true;
	}
	if (receptTime_ > 0.0f) { // 受付時間
		// 硬直秒数が0秒以上の場合タイマーを開始する
		receptTimer_.Start(receptTime_);
		isRecept_ = true;
	}

	// 有効秒数が0以下ならそもそもタイマーを無効化する
	if (attackEnableTime_ <= 0.0f) { attackDecisionTimer_.SetIsActive(false); }
	if (attackAssistEnableTime_ <= 0.0f) { attackAssistTimer_.SetIsActive(false); }

	// コライダーの有効状態を切っておく
	collider->isActive = false;

	// アニメーション名に何かしら入力されていれば
	if (animName_ != "") {
		// ループ設定
		anim->Loop(isLoop_);

		// アニメーションの再生を行う
		anim->Play(animName_, transitionTime_);
		// 再生速度指定
		anim->GetPlayBackSpeed() = animSpeed_;
	}

	// エフェクターの取得
	slashEffector_ = effector;
}

void Combo::Update(LWP::Resource::SkinningModel* model, LWP::Resource::Animation* anim, LWP::Object::Collision* collider, LWP::Object::Collider::Sphere* shape)
{
	// 攻撃判定用のタイマーが動作している場合のみ更新を行う
	if (attackDecisionTimer_.GetIsActive()) {
		// 攻撃判定に関する更新
		AttackActiveUpdate(model, collider, shape);
	}

	// 攻撃アシスト判定用のタイマーが動作している場合のみ更新を行う
	if (attackAssistTimer_.GetIsActive()) {
		// 攻撃アシスト判定に関する更新
		AttackAssistUpdate();
	}

	// 斬撃エフェクト用タイマーが動作している場合のみ更新を行う
	if (slashEffectTimer_.GetIsActive()) {
		// 攻撃アシスト判定に関する更新
		SlashEffectUpdate(model);
	}

	// 硬直時間タイマーが動作している場合のみ更新を行う
	if (stifnessTimer_.GetIsActive()) {
		// 硬直に関する更新
		StifnessTimeUpdate();
	}

	// コンボ受付判定用のタイマーが動作している場合のみ更新を行う
	if (receptTimer_.GetIsActive()) {
		// コンボ受付時間に関する更新
		ReceptTimeUpdate();
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

void Combo::NodeMenu(int& id, int& buttonID, Combo*& combo)
{
	// 削除フラグが立っているものの削除
	DeleteFunc(combo);

	// ボタンが選択されている場合色を変更する
	if (imGuiSelected_) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0, 255, 0, 1.0f));
	}

	// 名前がない場合エラーを吐いてしまうので名前がない場合NoNameと表示する
	std::string displayName = name_;
	if (displayName == "") { displayName = "No Name!"; }

	// ラジオボタンの表示
	if (ImGui::RadioButton(displayName.c_str(), &id, buttonID)) {
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

	ImGui::NewLine();
	// コンボ編集メニュー
	ImGui::Separator();
	ImGui::SeparatorText("Combo Edit Menu");
	ImGui::Separator();

	// コンボ名称の編集
	if (!isRoot_) { // 無操作状態のコンボで無ければ
		Base::ImGuiManager::InputText("Name", name_);
	}
	else { // 無操作状態のコンボであれば
		// 名称を変更出来ないことを伝える
		ImGui::Text("This Is RootCombo! You Can't Change Name");
	}

	ImGui::NewLine();

	// 開始条件の設定
	StartConditionSettings();

	// 派生優先度の設定
	PrioritySettings();

	// アニメーション関連の設定
	AnimSettings();

	// 攻撃判定の設定
	AttackSettings();

	// 硬直関係の設定
	StifnessSetiings();

	// コンボ受付関係の設定
	ReceptSettings();

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

void Combo::SameNameCount(const std::string& name, int& count)
{
	// コンボ名称取得、末尾に番号があれば取り外す
	std::string comboName = name_;
	while (isdigit(comboName.at(comboName.size() - 1)))
	{
		// 末尾の文字を削除
		comboName.pop_back();
	}

	// 同名オブジェクトがあった場合は加算
	if (comboName == name) {
		count++;
	}

	// 派生コンボに対しても同様の処理を行い、同名のコンボ数を求める
	for (Combo* c : childs_) {
		c->SameNameCount(name, count);
	}
}

void Combo::AddValue(LWP::Utility::JsonIO& json)
{
	// コンボ名でグループ開始、各パラメータの保存
	json.BeginGroup(name_)
		.AddValue("derivationPriority", &derivationProiority_)					// 派生優先度
		.AddValue("AnimName", &animName_)										// アニメーション名
		.AddValue("AnimPlaySpeed", &animSpeed_)									// アニメーション名
		.AddValue("TransitionTime", &transitionTime_)							// 遷移時間
		.AddValue("IsLoop", &isLoop_)											// ループ状態
		.AddValue("AttackStartTime", &attackStartTime_)							// 判定開始時間
		.AddValue("AttackEndTime", &attackEnableTime_)							// 判定有効時間
		.AddValue("FollowJointName", &followJointName_)							// 追従するジョイント名
		.AddValue("AttackColliderOffset", &attackColliderOffset_)				// オフセット
		.AddValue("AttackColliderRadius", &attackColliderRadius_)				// コライダーのサイズ
		.AddValue("StartSlashEffectTime", &startSlashEffectTime_)				// 斬撃エフェクトの開始時間
		.AddValue("SlashEffectOffset", &slashEffectOffset_)						// 斬撃エフェクトのオフセット
		.AddValue("SlashEffectRotate", &slashEffectRotate_)						// 斬撃エフェクトの回転角
		.AddValue("SlashEffectScale", &slashEffectScale_)						// 斬撃エフェクトのスケール
		.AddValue("PlaySlashEffectTime", &playSlashEffectTime_)					// 斬撃エフェクトの再生時間
		.AddValue("Damage", &damage_)											// 攻撃のダメージ量
		.AddValue("HitStopTime", &hitStopTime_)									// ヒットストップ秒数
		.AddValue("NockbackStrength", &nockbackStrength_)						// ノックバック強さ
		.AddValue("SheathDurabityLoss", &sheathDurabityLoss_)					// 鞘の耐久値減少量
		.AddValue("AttackAssistStartTime", &attackAssistStartTime_)				// 攻撃アシスト開始時間
		.AddValue("AttackAssistEnableTime", &attackAssistEnableTime_)			// 攻撃アシスト有効時間
		.AddValue("AttackAssistMoveAmount", &attackAssistMoveAmount_)			// 攻撃アシスト移動量
		.AddValue("StifnessTime", &stifnessTime_)								// 硬直時間
		.AddValue("ReceptTime", &receptTime_);									// 受付時間

	// 開始条件の保存
	int condCount = 1;
	for (LWP::Utility::ICondition* c : conditions_) {

		// 開始条件の保存
		c->AddValue(json, condCount);

		// カウントを進める
		condCount++;
	}

	// 派生クラスの情報保存
	for (Combo* c : childs_) {
		c->AddValue(json);
	}

	// このコンボのグループを終了する
	json.EndGroup();
}

Combo& Combo::CreateChild(const std::string& name)
{
	// 新規コンボの生成
	Combo* c = new Combo();
	// コンボの初期化
	c->Init(name);

	// 派生コンボ配列に追加
	childs_.push_back(std::move(c));
	
	// 生成下コンボの参照を返す
	return *c;
}

void Combo::SortByPriority()
{
	// 配列内の派生優先度を比較して昇順に並べ替える
	childs_.sort([](const Combo* c1, const Combo* c2) {
		return c1->GetDerivationPriority() < c2->GetDerivationPriority();
	});
}

void Combo::SortByPriorityAll()
{
	// 自身の派生コンボ配列の並び替え
	SortByPriority();

	// 全ての派生コンボ配列に対して並び替えを実行
	for (Combo* c : childs_) {
		c->SortByPriority();
	}
}

void Combo::DeleteThis()
{
	// 派生コンボ配列内の要素削除
	for (Combo* c : childs_) {
		delete c;
	}
	// 配列の要素クリア
	childs_.clear();

	// このコンボの削除フラグをONにする
	imGuiIsDelete_ = true;
}

void Combo::AddCondition(LWP::Utility::ICondition* condition)
{
	// 条件配列に新たな条件を追加する
	conditions_.push_back(std::move(condition));
}

void Combo::AttackActiveUpdate(LWP::Resource::SkinningModel* model, LWP::Object::Collision* collider, LWP::Object::Collider::Sphere* shape)
{
	// タイマーの更新
	attackDecisionTimer_.Update();

	// 攻撃判定タイマーが終了していれば
	if (attackDecisionTimer_.GetIsFinish()) {
		// 攻撃判定が有効になっていなければ
		if (!isAttackActive_) {
			// 攻撃判定有効
			isAttackActive_ = true;
			// 攻撃判定有効秒数でタイマー開始
			attackDecisionTimer_.Start(attackEnableTime_);
			// コライダーの有効判定設定
			collider->isActive = true;
		}
		else {
			// 攻撃判定無効
			isAttackActive_ = false;
			// 攻撃判定タイマーを非アクティブに
			attackDecisionTimer_.SetIsActive(false);
			// コライダーの有効判定設定
			collider->isActive = false;
		}
	}
	
	// 追従するジョイント名が指定されている場合
	if (followJointName_ != "") {
		// コライダーの追従設定
		collider->SetFollow(model, followJointName_);
	}
	else {
		// コライダーの追従設定
		collider->SetFollow(&model->worldTF);
	}

	// 回転行列を求める
	Math::Matrix4x4 rotMat = Math::Matrix4x4::CreateRotateXYZMatrix(model->worldTF.rotation);
	shape->position = attackColliderOffset_ * rotMat;
	// コライダーの半径調整
	shape->radius = attackColliderRadius_;
}

void Combo::AttackAssistUpdate()
{
	// タイマーの更新
	attackAssistTimer_.Update();

	// 攻撃アシスト判定タイマーが終了していれば
	if (attackAssistTimer_.GetIsFinish()) {
		// 攻撃アシスト判定が有効になっていなければ
		if (!isAttackAssistActive_) {
			// 攻撃判定有効
			isAttackAssistActive_ = true;
			// 攻撃判定終了秒数でタイマー開始
			attackAssistTimer_.Start(attackAssistEnableTime_);
		}
		else {
			// 攻撃アシスト判定無効
			isAttackAssistActive_ = false;
			// 攻撃判定タイマーを非アクティブに
			attackAssistTimer_.SetIsActive(false);
		}
	}
}

void Combo::SlashEffectUpdate(LWP::Resource::SkinningModel* model)
{
	// タイマーの更新
	slashEffectTimer_.Update();

	// タイマー終了時
	if (slashEffectTimer_.GetIsFinish()) {
		// 生成座標を求める
		Math::Vector3 GeneratePos{};
		GeneratePos = slashEffectOffset_;

		// クォータニオンをラジアンに変換する
		Math::Quaternion GenerateRotate{};
		GenerateRotate = Math::Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, slashEffectRotate_.x) * Math::Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, slashEffectRotate_.y) * Math::Quaternion::CreateFromAxisAngle({ 0.0f, 0.0f, 1.0f }, slashEffectRotate_.z);

		// 斬撃エフェクトを生成する
		slashEffector_->Create(GeneratePos, GenerateRotate, slashEffectScale_, playSlashEffectTime_, slashEffectOffset_);

		// タイマーを非アクティブ状態に
		slashEffectTimer_.SetIsActive(false);
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
		// 操作受付終了
		isRecept_ = false;
		// 受付時間タイマーを非アクティブに
		receptTimer_.SetIsActive(false);
	}
}

void Combo::DeleteFunc(Combo*& combo)
{
	// 削除するフラグが立っている派生コンボの削除をしておく
	std::list<Combo*>::iterator comboIt = childs_.begin();
	while (comboIt != childs_.end()) {
		Combo* c = *comboIt;
		if (c->GetIsDelete()) {
			delete c;
			comboIt = childs_.erase(comboIt);

			// このままだと参照範囲外としてエラーを吐くため編集対象その親に変更
			combo = this;
		}
		else {
			comboIt++;
		}
	}

	// 削除するフラグが立っている条件分岐の削除をしておく
	std::list<LWP::Utility::ICondition*>::iterator conditionIt = conditions_.begin();
	while (conditionIt != conditions_.end()) {
		LWP::Utility::ICondition* c = *conditionIt;
		if (c->GetIsDelete()) {
			delete c;
			conditionIt = conditions_.erase(conditionIt);
		}
		else {
			conditionIt++;
		}
	}
}

void Combo::StartConditionSettings()
{
	// 開始条件関連の設定
	ImGui::SeparatorText("Start Condition Settings");

	// 要素がない場合これ以降の処理を無視
	if (conditions_.empty()) {
		ImGui::NewLine();
		ImGui::Text("None Start Conditions!");
		ImGui::NewLine();
		return;
	}

	ImGui::Indent();

	// 個数カウント用
	int count = 1;

	// 全要素のImGuiの表示
	for (LWP::Utility::ICondition*& c : conditions_) {
		// 表示名称設定
		std::string name = "Condition " + std::to_string(count);

		// ツリーノードでImGuiを表示
		if (ImGui::TreeNode(name.c_str())) {
			c->DebugGUI();

			// ボタンを押した際に削除
			if (ImGui::Button("Delete")) {
				c->SetIsDelete(true);
			}

			ImGui::TreePop();
		}
		// カウント加算
		count++;
	}

	ImGui::Unindent();
	ImGui::NewLine();
}

void Combo::PrioritySettings()
{
	// アニメーション関係の設定
	ImGui::SeparatorText("Priority Settings");
	ImGui::Indent();

	// 派生優先度の設定
	ImGui::InputInt("Priority", &derivationProiority_);
	// 優先度が0以下になっていた場合0以上に補正
	if (derivationProiority_ < 0) { derivationProiority_ = 0; }

	ImGui::Unindent();
	ImGui::NewLine();
}

void Combo::AnimSettings()
{
	// アニメーション関係の設定
	ImGui::SeparatorText("Animation Settings");

	ImGui::Indent();

	// 再生されるアニメーションの設定
	Base::ImGuiManager::InputText("AnimName", animName_);

	// 遷移秒数の設定
	ImGui::DragFloat("TransitionTime", &transitionTime_, 0.01f, 0.0f);
	// 再生速度の設定
	ImGui::DragFloat("PlaySpeed", &animSpeed_, 0.01f, 0.0f);
	// ループ状態の設定
	ImGui::Checkbox("IsLoop", &isLoop_);

	ImGui::Unindent();
	ImGui::NewLine();
}

void Combo::AttackSettings()
{
	// 攻撃判定関連の設定
	ImGui::SeparatorText("Attack Settings");

	ImGui::Indent();

	// 開始時間
	ImGui::DragFloat("StartTime", &attackStartTime_, 0.01f, 0.0f);
	// 有効時間
	ImGui::DragFloat("EnableTime", &attackEnableTime_, 0.01f, 0.0f);

	// コライダーの追従対象設定
	Base::ImGuiManager::InputText("FollowJointName", followJointName_);

	// コライダーの半径調整
	ImGui::DragFloat3("ColliderOffset", &attackColliderOffset_.x, 0.01f, 0.0f);
	ImGui::DragFloat("ColliderRadius", &attackColliderRadius_, 0.01f, 0.0f);

	ImGui::Unindent();
	ImGui::NewLine();

	// 量関連の設定
	ImGui::SeparatorText("Amount Settings");

	ImGui::Indent();

	// ダメージ量の調整
	ImGui::DragFloat("Damage Amount", &damage_, 0.01f, 0.0f);
	// ヒットストップの調整
	ImGui::DragFloat("HitStopTime", &hitStopTime_, 0.1f, 0.0f);
	// ノックバック強さの調整
	ImGui::DragFloat("NockBack Strength", &nockbackStrength_, 0.01f, 0.0f);
	// 鞘の耐久減少量の調整
	ImGui::DragFloat("SheathDurabityLoss", &sheathDurabityLoss_, 0.1f, 0.0f);

	ImGui::Unindent();
	ImGui::NewLine();

	// 攻撃アシストの設定
	ImGui::SeparatorText("Attack Assist Settings");

	ImGui::Indent();

	// 攻撃アシストの開始時間
	ImGui::DragFloat("AssistStartTime", &attackAssistStartTime_, 0.01f, 0.0f);
	// 攻撃アシストの有効時間
	ImGui::DragFloat("AssistEnableTime", &attackAssistEnableTime_, 0.01f, 0.0f);

	// 移動量の指定
	ImGui::DragFloat3("AssistMoveAmount", &attackAssistMoveAmount_.x);

	ImGui::Unindent();
	ImGui::NewLine();

	// 斬撃エフェクトの設定
	ImGui::SeparatorText("SlashEffect Settings");

	ImGui::Indent();

	// 斬撃エフェクトの開始時間
	ImGui::DragFloat("StartSlashEffectTime", &startSlashEffectTime_, 0.01f, 0.0f);
	// 斬撃エフェクト演出時間
	ImGui::DragFloat("PlaySlashEffectTime", &playSlashEffectTime_, 0.01f, 0.0f);

	// オフセットの設定
	ImGui::DragFloat3("SlashEffectOffset", &slashEffectOffset_.x);
	ImGui::DragFloat3("SlashEffectRotate", &slashEffectRotate_.x);
	ImGui::DragFloat3("SlashEffectScale", &slashEffectScale_.x);

	ImGui::Unindent();
	ImGui::NewLine();
}

void Combo::StifnessSetiings()
{
	// 硬直関連の設定
	ImGui::SeparatorText("Stifness Settings");

	ImGui::Indent();

	// 硬直時間
	ImGui::DragFloat("StifnessTime", &stifnessTime_, 0.01f, 0.0f);

	ImGui::Unindent();
	ImGui::NewLine();
}

void Combo::ReceptSettings()
{
	// 受付時間関連の設定
	ImGui::SeparatorText("Recept Settings");

	ImGui::Indent();

	// 受付時間
	ImGui::DragFloat("ReceptTime", &receptTime_, 0.01f, 0.0f);

	// 終了時点で派生先が無かった場合自身に遷移するかどうか
	ImGui::Checkbox("IsReturnSelf", &isReturnSelf_);

	ImGui::Unindent();
	ImGui::NewLine();
}
