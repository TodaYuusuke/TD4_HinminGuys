#include "LockOn.h"
#include "../../Player.h"

LockOn::LockOn(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

void LockOn::Initialize() {
	isActive_ = false;
}

void LockOn::Update() {
	// 入力処理
	InputUpdate();

	// ロックオン可能状態の敵をリストに追加
	SearchLockOnEnemy();

	// ロックオン可能状態の条件から外れた敵をリストから除外する
	ClearLockOnList();
}

void LockOn::Reset() {
	lockOnDatas_.clear();
}

void LockOn::InputUpdate() {
	// LTでロックオン
	if (LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_LEFT_THUMB)) {
		if (!isActive_) {
			isActive_ = true;
		}
		else {
			isActive_ = false;
			Reset();
		}
	}
}

void LockOn::SearchLockOnEnemy() {
	for (const std::unique_ptr<IEnemy>& enemy : *enemies_) {
		// ロックオンリストに登録しているならスキップ
		auto result = std::find(lockedEnemyIDs_.begin(), lockedEnemyIDs_.end(), enemy->GetID());
		if (result != lockedEnemyIDs_.end()) { continue; }

		// ロックオン可能距離に敵がいないならスキップ
		LWP::Math::Vector3 p2e = enemy->GetPosition() - player_->GetWorldTF()->GetWorldPosition();
		float radius = kMaxRange;
		if ((p2e.x * p2e.x) + (p2e.y * p2e.y) + (p2e.z * p2e.z) > (radius * radius)) { continue; }

		// カメラの正面方向に敵がいないならスキップ
		LWP::Math::Vector3 cameraDir{ 0,0,1 };
		// 回転行列を求める
		Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pCamera_->worldTF.rotation);
		// 方向ベクトルを求める
		cameraDir = cameraDir * rotMatrix;
		if (IsObjectInOppositeDirection(enemy->GetPosition(), pCamera_->worldTF.translation, cameraDir)) { continue; }

		// スクリーン座標内にいるか
		if (!IsObjectInScreen(enemy->GetPosition())) { continue; }

#pragma region ロックオンリストに追加
		// ロックオンされたIDを登録
		lockedEnemyIDs_.push_back(enemy->GetID());
		// ロックオンした敵情報をリストに入れる
		LockOnData lockOnData = {
			enemy.get(),
			false
		};
		lockOnDatas_.push_back(lockOnData);
#pragma endregion
	}
}

void LockOn::SearchNearEnemy() {
	// ロックオンリスト内の敵で自機に最も近い敵を最初にロックオン
	for (int i = 0; i < lockOnDatas_.size(); i++) {

	}
}

void LockOn::ClearLockOnList() {
	for (int i = 0; i < lockOnDatas_.size(); i++) {
		// ロックオン可能距離に敵がいないならリストから除外
		LWP::Math::Vector3 p2e = lockOnDatas_[i].enemyData->GetPosition() - player_->GetWorldTF()->GetWorldPosition();
		float radius = kMaxRange;
		if ((p2e.x * p2e.x) + (p2e.y * p2e.y) + (p2e.z * p2e.z) > (radius * radius)) {
			// ロックオン対象のIDを消す
			lockedEnemyIDs_.erase(std::remove(lockedEnemyIDs_.begin(), lockedEnemyIDs_.end(), lockOnDatas_[i].enemyData->GetID()), lockedEnemyIDs_.end());
			// ロックオンリストから除外
			lockOnDatas_.erase(lockOnDatas_.begin() + i);
			continue;
		}

		// カメラの正面方向に敵がいないならリストから除外
		LWP::Math::Vector3 cameraDir{ 0,0,1 };
		// 回転行列を求める
		Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pCamera_->worldTF.rotation);
		// 方向ベクトルを求める
		cameraDir = cameraDir * rotMatrix;
		if (IsObjectInOppositeDirection(lockOnDatas_[i].enemyData->GetPosition(), pCamera_->worldTF.translation, cameraDir)) {
			// ロックオン対象のIDを消す
			lockedEnemyIDs_.erase(std::remove(lockedEnemyIDs_.begin(), lockedEnemyIDs_.end(), lockOnDatas_[i].enemyData->GetID()), lockedEnemyIDs_.end());
			// ロックオンリストから除外
			lockOnDatas_.erase(lockOnDatas_.begin() + i);
			continue;
		}

		// スクリーン座標内にいないならリストから除外
		if (!IsObjectInScreen(lockOnDatas_[i].enemyData->GetPosition())) {
			// ロックオン対象のIDを消す
			lockedEnemyIDs_.erase(std::remove(lockedEnemyIDs_.begin(), lockedEnemyIDs_.end(), lockOnDatas_[i].enemyData->GetID()), lockedEnemyIDs_.end());
			// ロックオンリストから除外
			lockOnDatas_.erase(lockOnDatas_.begin() + i);
			continue;
		}
	}
}

Vector2 LockOn::ConvertWorld2Screen(LWP::Math::Vector3 worldPos) {
	LWP::Math::Vector3 result = worldPos;
	result += { 0.000001f, 0.000001f, 0.000001f };

	// ビューポート行列
	LWP::Math::Matrix4x4 matViewport = LWP::Math::Matrix4x4::CreateViewportMatrix(0, 0, LWP::Info::GetWindowWidthF(), LWP::Info::GetWindowHeightF(), 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	LWP::Math::Matrix4x4 matViewProjectionViewport{};
	matViewProjectionViewport = pCamera_->GetViewProjection() * matViewport;
	// ワールド→スクリーン座標変換
	result = Transforms(result, matViewProjectionViewport);

	return LWP::Math::Vector2(result.x, result.y);
}

bool LockOn::IsObjectInScreen(LWP::Math::Vector3 worldPos) {
	// 敵がレティクルの一定範囲にいるならロックオン(判定はスクリーン座標で行う)
	Vector2 objectPos = ConvertWorld2Screen(worldPos);
	// 対象が画面に映っているか
	if (objectPos.x >= 0.0f && objectPos.x <= LWP::Info::GetWindowWidthF() &&
		objectPos.y >= 0.0f && objectPos.y <= LWP::Info::GetWindowHeightF()) {
		return true;
	}

	return false;
}

bool LockOn::IsObjectInOppositeDirection(const LWP::Math::Vector3& objectPosition, const LWP::Math::Vector3& cameraPosition, const LWP::Math::Vector3& cameraDirection) {
	LWP::Math::Vector3 cameraToEnemyDirection = (cameraPosition - objectPosition).Normalize();
	float dot = LWP::Math::Vector3::Dot((cameraDirection).Normalize(), cameraToEnemyDirection);

	// カメラの正面方向にいる
	if (dot < 0.0f) {
		return false;
	}

	// カメラの背後にいる
	return true;
}

LWP::Math::Vector3 LockOn::Transforms(const LWP::Math::Vector3& vector, const LWP::Math::Matrix4x4& matrix) {
	LWP::Math::Vector3 result{};
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