#pragma once
#include <Adapter.h>
#include <list>
#include "Combo.h"

/// <summary>
/// 衝突時の処理追加用
/// </summary>
namespace LWP::Utility::ComboEnum {
	enum CollisionState {
		ENTER,
		STAY,
		EXIT
	};
}

/// <summary>
/// コンボツリークラス
/// </summary>
class ComboTree
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ComboTree();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ComboTree();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="fileName">読み込み保存するファイル名<param>
	/// <param name="model">アニメーションさせるモデル</param>
	/// <param name="anim">アニメーション</param>
	void Init(const std::string& fileName, LWP::Resource::SkinningModel* model, LWP::Resource::Animation* anim);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグのImGuiを表示する関数
	/// </summary>
	void DebugGUI();

	/// <summary>
	/// 無操作状態コンボに強制的に戻す関数
	/// </summary>
	void ResetCombo();

public: // アクセッサ等

	/// <summary>
	/// コライダーマスクのセッター
	/// </summary>
	/// <param name="maskID">マスクするマスクID</param>
	/// <param name="hitID">当たるマスクID</param>
	void SetColliderMaskFrag(uint32_t maskID, uint32_t hitID);

	/// <summary>
	/// 衝突時の処理を追加
	/// </summary>
	/// <param name="function">追加する衝突時処理</param>
	void AddCollisionLamda(int collisionState, LWP::Object::Collision::OnHitFunction function);

	/// <summary>
	/// 編集モード中かどうかのゲッター
	/// </summary>
	/// <returns>編集モード中か</returns>
	bool GetIsEditingMode() { return enableEditMode_; }

	/// <summary>
	/// 次のコンボ受付可能フラグの状態セッター
	/// </summary>
	/// <param name="isRecept">受付可能フラグ状態</param>
	void SetIsRecept(const bool isRecept) { isRecept_ = isRecept; }

	/// <summary>
	/// 現在のコンボが大元のコンボであるかどうかのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsThisRoot() { return nowCombo_->GetIsRoot(); }

	/// <summary>
	/// 現在コンボのダメージ量ゲッター
	/// </summary>
	/// <returns>ダメージ量</returns>
	float GetDamage() { return nowCombo_->GetDamage(); }

	/// <summary>
	/// 現在コンボのノックバック強さゲッター
	/// </summary>
	/// <returns>ノックバック強さ</returns>
	float GetNockBackStrength() { return nowCombo_->GetNockBackStrength(); }

	/// <summary>
	/// 現在コンボの鞘の耐久値減少量ゲッター
	/// </summary>
	/// <returns>鞘の耐久値減少量</returns>
	float GetSheathDurabityLoss() { return nowCombo_->GetSheathDurabityLoss(); }

	/// <summary>
	/// 攻撃アシストの有効状態ゲッター
	/// </summary>
	/// <returns>攻撃アシストの有効状態</returns>
	bool GetIsEnableAttackAssist() { return nowCombo_->GetIsAttackAssistActive(); }

	/// <summary>
	/// 現在コンボの硬直状態ゲッター
	/// </summary>
	/// <returns>硬直状態</returns>
	bool GetIsStiffness();

	/// <summary>
	/// コンボの受付状態のゲッター
	/// </summary>
	/// <returns>コンボの受付状態</returns>
	bool GetIsRecept();

	/// <summary>
	/// 受付時間終了遷移確認トリガーの状態ゲッター
	/// </summary>
	/// <returns>受付時間終了遷移確認</returns>
	bool GetIsReceptEndTrigger() { return isReceptEndTrigger_; }
	
	/// <summary>
	/// 攻撃アシストの移動量ゲッター
	/// </summary>
	/// <returns>攻撃アシストの移動量</returns>
	LWP::Math::Vector3 GetAttackAssistMoveAmount() { return nowCombo_->GetAttackAssistMoveAmount(); }

private: // プライベートなメンバ関数

	/// <summary>
	/// コライダー生成関数
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// 新規作成弥保存を行うメニュー
	/// </summary>
	void FileMenu();

	/// <summary>
	/// ノードエディタメニュー
	/// </summary>
	void NodeMenu();

	/// <summary>
	/// コンボの保存関数
	/// </summary>
	void SaveCombo();
	
	/// <summary>
	/// コンボのロード関数
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LoadCombo(const std::string& fileName);

	/// <summary>
	/// <エディタ用> 同名コンボがいくつあるかのカウンター
	/// </summary>
	/// <param name="name">検証する名称</param>
	/// <returns>同名コンボ数</returns>
	int GetSameNameCount(const std::string& name);

	/// <summary>
	/// アニメーション名を設定をやりやすくするメニュー
	/// </summary>
	void AnimNameEasySetter();

	/// <summary>
	/// <エディタ用> 派生コンボ生成関数
	/// </summary>
	void CreateChildMenu();

	/// <summary>
	/// <エディタ用> 開始条件生成関数
	/// </summary>
	void CreateConditionMenu();

	/// <summary>
	/// <エディタ用> 開始条件の入力に関するメニュー関数
	/// </summary>
	void InputMenu();

	/// <summary>
	/// 削除メニュー
	/// </summary>
	void DeletePopUp();

private: // メンバ変数

	// アニメーションさせるモデル
	LWP::Resource::SkinningModel* animModel_ = nullptr;
	// アニメーション本体
	LWP::Resource::Animation* anim_ = nullptr;

	// コライダー
	LWP::Object::Collision collider_;
	LWP::Object::Collider::Capsule& capsule_;

	// 現在のコンボ
	Combo* nowCombo_ = nullptr;
	// 次のコンボ
	Combo* nextCombo_ = nullptr;

	// 次のコンボの受付可能フラグ
	bool isRecept_ = true;

	// 受付終了で遷移したかどうかのトリガー
	bool isReceptEndTrigger_ = false;

#pragma region エディタ用変数

	// 保存先ファイル名
	std::string fileName_{};

	// jsonIO
	LWP::Utility::JsonIO jsonIO_;

	// 編集モード有効フラグ
	bool enableEditMode_ = false;

	// 選択中のコンボID
	int selectedID_ = 0;

	// 無操作状態のコンボ
	Combo rootCombo_;

	// 編集中のコンボ
	Combo* editingCombo_ = nullptr;

	// 生成する派生コンボ名称
	std::string imGuiChildComboName_;

	// 削除確認用ポップアップフラグ
	bool imGuiIsOpenPopUp_ = false;

#pragma endregion

};

