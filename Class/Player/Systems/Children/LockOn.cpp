#include "LockOn.h"
#include "../../Player.h"
#include "../../../Camera/FollowCamera.h"

using namespace LWP::Math;

// ロックオンできる範囲
float LockOn::kMaxRange = 50.0f;

LockOn::LockOn(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

void LockOn::Initialize() {
	isActive_ = false;
	isPreActive_ = false;
	isChangeLockOn_ = false;
	isChangeLocked_ = false;

	// ロックオン中のレティクル
	lockOnUI_.defaultPos = { 0,1,0 };
	lockOnUI_.defaultAnchorPoint = { 0.5f, 0.5f };
	lockOnUI_.sprite.LoadTexture("lockOnReticle.png");
	lockOnUI_.sprite.anchorPoint = lockOnUI_.defaultAnchorPoint;
	lockOnUI_.sprite.isActive = false;
	lockOnUI_.enableLockOnObj.isActive = false;

	// jsonで保存している値
	CreateJsonFIle();
}

void LockOn::Update() {
	// ロックオン可能状態の敵をリストに追加
	SearchLockOnEnemy();

	// ロックオン可能状態の条件から外れた敵をリストから除外する
	ClearLockOn();

	// 最も近い敵をロックオン
	SearchNearEnemy();

	// ロックオン時のレティクル座標を更新
	LockOnReticleUpdate();

	isChangeLocked_ = isChangeLockOn_;

	// ロックオン対象の変更処理
	ChangeLockOnTarget();

	isPreActive_ = isActive_;
}

void LockOn::Reset() {
	followCamera_->FinishLockOn();
	lockedEnemyIDs_.clear();
	lockOnEnemy_ = nullptr;
	isActive_ = false;
	isChangeLockOn_ = false;
	isChangeLocked_ = false;
	inputCameraRotateY_ = 0.0f;
}

void LockOn::DebugGUI() {
	if (ImGui::TreeNode("LockOn")) {
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			ImGui::TreePop();
		}

		ImGui::Checkbox("IsLockOn", &isActive_);
		ImGui::Checkbox("IsChangeLockOnTarget", &isChangeLockOn_);
		ImGui::Checkbox("IsChangeLocked", &isChangeLocked_);
		ImGui::TreePop();
	}
}

void LockOn::CreateJsonFIle() {
	json_.Init("LockOnData.json");
	json_.AddValue("Range", &kMaxRange)
		.CheckJsonFile();
}

void LockOn::Command() {
	if (!isActive_) {
		isActive_ = true;
		isChangeLockOn_ = true;
	}
	else { Reset(); }
}

void LockOn::ChangeLockOnTarget() {
	if (!isActive_) { return; }

#pragma region ロックオン時のみ左右どちらに入力があったかを知るため
	// 回転する向き
	Vector2 dir = { 0.0f,0.0f };
	// キーボード入力
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		dir.y += 1.0f;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		dir.y -= 1.0f;
	}
	// コントローラーでの回転
	dir.x += LWP::Input::Pad::GetRStick().x;
#pragma endregion

	inputCameraRotateY_ = dir.x;

	// ロックオン中に入力があった
	if (inputCameraRotateY_ != 0.0f) {
		isChangeLockOn_ = true;
	}
	else {
		isChangeLockOn_ = false;
	}
}

void LockOn::SearchLockOnEnemy() {
	for (IEnemy* enemy : *enemies_) {
		// ロックオン可能状態の敵ならスキップ(多重にロックオンしないようにするため)
		if (enemy->GetIsLocked()) { continue; }

		// ロックオン可能距離に敵がいないならスキップ
		Vector3 p2e = enemy->GetPosition() - player_->GetWorldTF()->GetWorldPosition();
		float radius = kMaxRange;
		if ((p2e.x * p2e.x) + (p2e.y * p2e.y) + (p2e.z * p2e.z) > (radius * radius)) {
			// ロックオンしている敵がロックオン可能距離から外れたらZ注目終了
			if (isActive_ && lockOnEnemy_ == enemy) { Reset(); }
			continue;
		}

		// カメラの正面方向に敵がいないならスキップ
		Vector3 cameraDir{ 0,0,1 };
		// 回転行列を求める
		Matrix4x4 rotMatrix = Matrix4x4::CreateRotateXYZMatrix(pCamera_->worldTF.rotation);
		// 方向ベクトルを求める
		cameraDir = cameraDir * rotMatrix;
		if (IsObjectInOppositeDirection(enemy->GetPosition(), pCamera_->worldTF.translation, cameraDir)) { continue; }

		// スクリーン座標内にいるか
		if (!IsObjectInScreen(enemy->GetPosition())) { continue; }

		// 現在ロックオン中の敵は追加しない
		if (enemy == lockOnEnemy_) { continue; }

		// 敵をロックオン可能状態にする
		enemy->SetIsLocked(true);
		// ロックオン可能UIを設置
		LockOnData lockOnData;
		lockOnData.enemyData = enemy;
		lockOnData.ui.sprite.LoadTexture("arrow.png");
		lockOnData.ui.sprite.anchorPoint = { 0.5f,0.5f };
		lockOnData.ui.enableLockOnObj.LoadTexture("arrow.png");
		lockOnData.ui.enableLockOnObj.Init();
		lockOnData.ui.enableLockOnObj.name = "billbard";
		lockOnData.ui.enableLockOnObj.anchorPoint = { 0.5f, 0.5f };
		lockOnData.ui.enableLockOnObj.isActive = false;
		lockOnEnableEnemies_.push_back(lockOnData);
	}
}

void LockOn::ClearLockOn() {
	for (IEnemy* enemy : *enemies_) {
		// ロックオン可能状態の敵じゃないならスキップ
		if (!enemy->GetIsLocked()) { continue; }

		// ロックオン可能距離に敵がいないならリストから除外
		Vector3 p2e = enemy->GetPosition() - player_->GetWorldTF()->GetWorldPosition();
		float radius = kMaxRange;
		if ((p2e.x * p2e.x) + (p2e.y * p2e.y) + (p2e.z * p2e.z) > (radius * radius)) {
			enemy->SetIsLocked(false);
			auto it = std::find(lockOnEnableEnemies_.begin(), lockOnEnableEnemies_.end(), enemy);
			if (it != lockOnEnableEnemies_.end()) {
				lockOnEnableEnemies_.erase(std::remove(lockOnEnableEnemies_.begin(), lockOnEnableEnemies_.end(), enemy), lockOnEnableEnemies_.end());
			}
			continue;
		}

		// カメラの正面方向に敵がいないならリストから除外
		Vector3 cameraDir{ 0,0,1 };
		// 回転行列を求める
		Matrix4x4 rotMatrix = Matrix4x4::CreateRotateXYZMatrix(pCamera_->worldTF.rotation);
		// 方向ベクトルを求める
		cameraDir = cameraDir * rotMatrix;
		if (IsObjectInOppositeDirection(enemy->GetPosition(), pCamera_->worldTF.translation, cameraDir)) {
			enemy->SetIsLocked(false);
			auto it = std::find(lockOnEnableEnemies_.begin(), lockOnEnableEnemies_.end(), enemy);
			if (it != lockOnEnableEnemies_.end()) {
				lockOnEnableEnemies_.erase(std::remove(lockOnEnableEnemies_.begin(), lockOnEnableEnemies_.end(), enemy), lockOnEnableEnemies_.end());
			}
			continue;
		}

		// スクリーン座標内にいないならリストから除外
		if (!IsObjectInScreen(enemy->GetPosition())) {
			enemy->SetIsLocked(false);
			auto it = std::find(lockOnEnableEnemies_.begin(), lockOnEnableEnemies_.end(), enemy);
			if (it != lockOnEnableEnemies_.end()) {
				lockOnEnableEnemies_.erase(std::remove(lockOnEnableEnemies_.begin(), lockOnEnableEnemies_.end(), enemy), lockOnEnableEnemies_.end());
			}

			continue;
		}
	}
}

void LockOn::SearchNearEnemy() {
	if (!isChangeLocked_ && isChangeLockOn_) {
		for (IEnemy* enemy : *enemies_) {
			// ロックオン可能状態の敵じゃないならスキップ
			if (!enemy->GetIsLocked()) { continue; }

			// 一度でもロックオンしたことがあるならスキップ
			/*bool isLockedEnemy = false;
			for (int i = 0; i < lockedEnemyIDs_.size(); i++) {
				if (enemy->GetID() == lockedEnemyIDs_[i]) {
					isLockedEnemy = true;
					break;
				}
			}
			if (isLockedEnemy) { continue; }*/

			// 入力した方向に敵がいるならロックオン対象を変更
			if (inputCameraRotateY_ != 0.0f && !lockedEnemyIDs_.empty()) {
				// 自機の右方向を基準とする
				Vector3 dir = { 0,0,1 };
				// 回転行列を求める
				Matrix4x4 rotMatrix = Matrix4x4::CreateRotateXYZMatrix(pCamera_->worldTF.rotation);
				// 方向ベクトルを求める
				dir = dir * rotMatrix;
				dir = Vector3::Cross(Vector3{ 0,1,0 }, dir);
				// 値が-なら左に敵がいる
				float dot = Vector3::Dot((dir).Normalize(), (enemy->GetPosition() - player_->GetWorldTF()->GetWorldPosition()).Normalize());
				// 異なる符号ならロックオンしない
				if (std::signbit(inputCameraRotateY_) != std::signbit(dot)) {
					continue;
				}
			}
			// ロックオン開始
			StartLockOn(enemy);

			break;
		}
	}
}

void LockOn::StartLockOn(IEnemy* enemy) {
	// すでに存在しない場合だけ追加する
	if (std::find(lockedEnemyIDs_.begin(), lockedEnemyIDs_.end(), enemy->GetID()) == lockedEnemyIDs_.end()) {
		lockedEnemyIDs_.push_back(enemy->GetID());
	}

	// ロックオン対象をセット
	lockOnEnemy_ = enemy;
	// ロックオン対象にZ注目開始
	followCamera_->StartLockOn(lockOnEnemy_->GetWorldTF());
	// ロックオン中の敵は"ロックオン可能状態"を外す
	enemy->SetIsLocked(false);
	isChangeLocked_ = false;
}

void LockOn::LockOnReticleUpdate() {
	lockOnUI_.sprite.isActive = false;
	// Z注目をしている敵がいるなら専用UIを表示
	if (lockOnEnemy_) {
		lockOnUI_.sprite.isActive = true;
		// 敵の座標をスクリーン座標に変換
		Vector2 screenPos = ConvertWorld2Screen(lockOnEnemy_->GetPosition() + lockOnUI_.defaultPos);
		// レティクルスプライトの座標を更新
		lockOnUI_.sprite.worldTF.translation = {
			screenPos.x,
			screenPos.y,
			0
		};

		// Z注目中の敵を"ロックオン可能状態"リストから除外
		auto it = std::find(lockOnEnableEnemies_.begin(), lockOnEnableEnemies_.end(), lockOnEnemy_);
		if (it != lockOnEnableEnemies_.end()) {
			lockOnEnableEnemies_.erase(std::remove(lockOnEnableEnemies_.begin(), lockOnEnableEnemies_.end(), lockOnEnemy_), lockOnEnableEnemies_.end());
		}
	}

	// ロックオン可能な敵にUIを表示
	for (LockOnData& lockOnEnemy : lockOnEnableEnemies_) {
		// 敵がロックオン可能状態でないならスキップ
		if (!lockOnEnemy.enemyData->GetIsLocked()) { continue; }

		// 敵の座標をスクリーン座標に変換
		Vector2 screenPos = ConvertWorld2Screen(lockOnEnemy.enemyData->GetPosition() + Vector3{ 0, 1, 0 });
		// レティクルスプライトの座標を更新
		lockOnEnemy.ui.sprite.worldTF.translation = {
			screenPos.x,
			screenPos.y,
			0
		};
		lockOnEnemy.ui.sprite.isActive = false;
		lockOnEnemy.ui.enableLockOnObj.worldTF.Parent(lockOnEnemy.enemyData->GetWorldTF());
		lockOnEnemy.ui.enableLockOnObj.worldTF.translation = Vector3{0.0f, 0.6f, 0.0f};
		lockOnEnemy.ui.enableLockOnObj.isActive = true;
	}
}

Vector2 LockOn::ConvertWorld2Screen(Vector3 worldPos) {
	Vector3 result = worldPos;
	result += { 0.000001f, 0.000001f, 0.000001f };

	// ビューポート行列
	Matrix4x4 matViewport = Matrix4x4::CreateViewportMatrix(0, 0, LWP::Info::GetWindowWidthF(), LWP::Info::GetWindowHeightF(), 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport{};
	matViewProjectionViewport = pCamera_->GetViewProjection() * matViewport;
	// ワールド→スクリーン座標変換
	result = Transforms(result, matViewProjectionViewport);

	return Vector2{ result.x, result.y };
}

bool LockOn::IsObjectInScreen(Vector3 worldPos) {
	// 敵がレティクルの一定範囲にいるならロックオン(判定はスクリーン座標で行う)
	Vector2 objectPos = ConvertWorld2Screen(worldPos);
	// 対象が画面に映っているか
	if (objectPos.x >= 0.0f && objectPos.x <= LWP::Info::GetWindowWidthF() &&
		objectPos.y >= 0.0f && objectPos.y <= LWP::Info::GetWindowHeightF()) {
		return true;
	}

	return false;
}

bool LockOn::IsObjectInOppositeDirection(const Vector3& objectPosition, const Vector3& cameraPosition, const LWP::Math::Vector3& cameraDirection) {
	Vector3 cameraToEnemyDirection = (cameraPosition - objectPosition).Normalize();
	float dot = Vector3::Dot((cameraDirection).Normalize(), cameraToEnemyDirection);

	// カメラの正面方向にいる
	if (dot < 0.0f) {
		return false;
	}

	// カメラの背後にいる
	return true;
}

Vector3 LockOn::Transforms(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}