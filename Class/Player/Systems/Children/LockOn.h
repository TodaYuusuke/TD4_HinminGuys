#pragma once
#include "../ISystem.h"
#include "../../../Enemy/IEnemy.h"
#include <vector>

class FollowCamera;
class LockOn : public ISystem {
public:// 構造体
	struct LockOnUI {
		LWP::Primitive::Sprite sprite;		// ロックオン可能UIのスプライト
		LWP::Primitive::Billboard3D enableLockOnObj;
		LWP::Object::TransformQuat offset;	// 補間の値を格納
		Vector3 defaultPos;					// ロックオン可能UIの初期座標
		Vector3 defaultScale;				// ロックオン可能UIの初期サイズ
		Vector2 defaultAnchorPoint;			// ロックオン可能UIの初期中心点
	};

	// ロックオン時に必要な情報
	struct LockOnData {
		IEnemy* enemyData;					// 敵の情報
		LockOnUI ui;

		// 同じ敵をロックオンしようとしているかを判別するときに使う
		bool operator==(IEnemy* other)const {
			return enemyData == other;
		}
	};

public:
	// コンストラクタ
	LockOn(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~LockOn() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 全ての数値をリセット
	/// </summary>
	void Reset() override;

	/// <summary>
	/// デバッグ用のタブを表示(Debug時のみ)
	/// </summary>
	void DebugGUI() override;

	/// <summary>
	/// jsonファイルの作成
	/// </summary>
	void CreateJsonFIle() override;

	/// <summary>
	/// ロックオンコマンド
	/// </summary>
	void Command();

private:
	/// <summary>
	/// ロックオン対象を変える
	/// </summary>
	void ChangeLockOnTarget();

	/// <summary>
	/// ロックオン可能範囲の敵を索敵
	/// </summary>
	void SearchLockOnEnemy();

	/// <summary>
	/// ロックオン情報を初期化する
	/// </summary>
	void ClearLockOn();

	/// <summary>
	/// 最も近い敵を索敵
	/// </summary>
	void SearchNearEnemy();

	/// <summary>
	/// ロックオン開始時に必要な情報
	/// </summary>
	void StartLockOn(IEnemy* enemy);

	/// <summary>
	/// ロックオン時のレティクルの更新処理
	/// </summary>
	void LockOnReticleUpdate();

	// ワールド座標からスクリーン座標に変換
	Vector2 ConvertWorld2Screen(Vector3 worldPos);

	bool IsObjectInScreen(Vector3 worldPos);

	// カメラの正面方向を算出
	bool IsObjectInOppositeDirection(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraDirection);

	Vector3 Transforms(const Vector3& vector, const Matrix4x4& matrix);

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 現在ロックオンしている敵の情報を取得
	/// </summary>
	/// <returns></returns>
	IEnemy* GetCurrentLockOnTarget() { return lockOnEnemy_;	}
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 敵のリストを設定
	/// </summary>
	/// <param name="enemyList">敵のリスト</param>
	void SetEnemyList(std::list<IEnemy*>* enemyList) { enemies_ = enemyList; }

	/// <summary>
	/// 追従カメラのポインタを設定
	/// </summary>
	/// <param name="followCamera">追従カメラのポインタ</param>
	void SetFollowCamera(FollowCamera* followCamera) { followCamera_ = followCamera; }
#pragma endregion

private:// jsonで保存する値
	// ロックオンできる範囲
	static float kMaxRange;

private:// 外部からポインタをもらう変数
	// 敵のリスト
	const std::list<IEnemy*>* enemies_;
	// 追従カメラ
	FollowCamera* followCamera_;

private:
	// ロックオンされたことのある敵のID
	std::vector<uint32_t> lockedEnemyIDs_;
	// ロックオンできる敵
	std::vector<LockOnData> lockOnEnableEnemies_;

	// 現在ロックオンされている敵の情報
	IEnemy* lockOnEnemy_;

	LockOnUI lockOnUI_;

	// ロックオン可能数
	int lockOnNum_;

	// カメラ回転、左右どちらに入力があったかを受け取る
	float inputCameraRotateY_;

	// ロックオン対象を変えるか
	bool isChangeLockOn_;
	bool isChangeLocked_;
};