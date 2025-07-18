#include "MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

namespace MathFunc {
	Quaternion LookRotation(const Vector3& dirVec) {
		// 向かせたい方向ベクトル
		Vector3 dir = dirVec.Normalize();
		// デフォルトの方向ベクトル
		Vector3 norm = { 0,0,1 };
		// 二つの方向ベクトルから角度算出
		float theta = std::acosf(Vector3::Dot(norm, dir)) / 2.0f;
		Vector3 cross = Vector3::Cross(norm, dir).Normalize();

		// 計算結果
		Quaternion q = {
			cross.x * std::sin(theta),
			cross.y * std::sin(theta),
			cross.z * std::sin(theta),
			std::cos(theta)
		};
		return q;
	}

	Quaternion HorizontalBillboard(const Vector3& cameraPos, const Vector3& targetPos) {
		Vector3 dir = cameraPos - targetPos;
		dir.y = 0;  // 水平成分だけ使う
		Quaternion result = LookRotation(dir);

		return result;
	}

	Quaternion StretchedBillboard(const Vector3& targetPos, const Vector3& vel, const Vector3& cameraPos) {
		Vector3 pos = targetPos;
		Vector3 cam = cameraPos;

		// forward = 速度方向（Z+）
		Vector3 forward = vel.Normalize();

		// camera direction（Billboard基準）
		Vector3 toCam = (cam - pos).Normalize();

		// right = forward × toCam
		Vector3 right = Vector3::Cross(forward, toCam).Normalize();

		// up = right × forward
		Vector3 up = Vector3::Cross(right, forward);

		// 回転行列を構築（列ベース）
		Matrix4x4 rotMatrix = {
			//right,    // X
			//up,       // Y
			//forward,  // Z
			//XMVectorSet(0, 0, 0, 1)
		};
		rotMatrix.m[0][0] = right.x;
		rotMatrix.m[0][1] = right.y;
		rotMatrix.m[0][2] = right.z;
		rotMatrix.m[0][3] = 0.0f;

		rotMatrix.m[1][0] = up.x;
		rotMatrix.m[1][1] = up.y;
		rotMatrix.m[1][2] = up.z;
		rotMatrix.m[1][3] = 0.0f;

		rotMatrix.m[2][0] = forward.x;
		rotMatrix.m[2][1] = forward.y;
		rotMatrix.m[2][2] = forward.z;
		rotMatrix.m[2][3] = 0.0f;

		rotMatrix.m[3][0] = 0.0f;
		rotMatrix.m[3][1] = 0.0f;
		rotMatrix.m[3][2] = 0.0f;
		rotMatrix.m[3][3] = 1.0f;


		// クォータニオンへ変換
		Quaternion q = QuaternionFromMatrix(rotMatrix);

		return q;
	}

	/*Quaternion QuaternionRotateMatrix(const Matrix4x4& m) {
		Quaternion q;
		float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];

		if (trace > 0.0f) {
			float s = sqrtf(trace + 1.0f) * 2.0f;
			q.w = 0.25f * s;
			q.x = (m.m[2][1] - m.m[1][2]) / s;
			q.y = (m.m[0][2] - m.m[2][0]) / s;
			q.z = (m.m[1][0] - m.m[0][1]) / s;
		}
		else {
			if (m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2]) {
				float s = sqrtf(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]) * 2.0f;
				q.w = (m.m[2][1] - m.m[1][2]) / s;
				q.x = 0.25f * s;
				q.y = (m.m[0][1] + m.m[1][0]) / s;
				q.z = (m.m[0][2] + m.m[2][0]) / s;
			}
			else if (m.m[1][1] > m.m[2][2]) {
				float s = sqrtf(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]) * 2.0f;
				q.w = (m.m[0][2] - m.m[2][0]) / s;
				q.x = (m.m[0][1] + m.m[1][0]) / s;
				q.y = 0.25f * s;
				q.z = (m.m[1][2] + m.m[2][1]) / s;
			}
			else {
				float s = sqrtf(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]) * 2.0f;
				q.w = (m.m[1][0] - m.m[0][1]) / s;
				q.x = (m.m[0][2] + m.m[2][0]) / s;
				q.y = (m.m[1][2] + m.m[2][1]) / s;
				q.z = 0.25f * s;
			}
		}

		return q;
	}*/

	float ExponentialInterpolateF(const float& current, const float& target, float damping) {
		float factor = 1.0f - std::exp(-damping);
		return current + (target - current) * factor;
	}

	float GetRotationAngleFromMatrix(const Matrix4x4& m) {
		// 回転部分のトレースを計算
		float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];

		// トレースから cosθ を求める
		float cosTheta = (trace - 1.0f) * 0.5f;

		// 安全のため -1〜1 にクランプ
		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		// θ = arccos( (trace - 1) / 2 )
		float theta = std::acos(cosTheta);

		return theta; // ラジアン単位
	}

	Quaternion QuaternionFromMatrix(const Matrix4x4& m) {
		float theta = GetRotationAngleFromMatrix(m); // ラジアン
		float sinTheta = std::sin(theta);

		// 小さすぎるときは無回転扱い
		if (std::abs(sinTheta) < 1e-6f) {
			return { 0, 0, 0, 1 }; // 単位クォータニオン
		}

		// 回転軸（normalized）
		float ux = (m.m[2][1] - m.m[1][2]) / (2.0f * sinTheta);
		float uy = (m.m[0][2] - m.m[2][0]) / (2.0f * sinTheta);
		float uz = (m.m[1][0] - m.m[0][1]) / (2.0f * sinTheta);

		float halfTheta = theta * 0.5f;
		float sinHalf = std::sin(halfTheta);
		float cosHalf = std::cos(halfTheta);

		Quaternion q;
		q.x = ux * sinHalf;
		q.y = uy * sinHalf;
		q.z = uz * sinHalf;
		q.w = cosHalf;

		return q;
	}

	float GetYawFromQuaternion(const LWP::Math::Quaternion& q) {
		// Yaw（Y軸まわりの回転）を取り出す
		float siny_cosp = 2.0f * (q.w * q.y + q.z * q.x);
		float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
		return std::atan2(siny_cosp, cosy_cosp); // ラジアン
	}

	LWP::Math::Vector3 Abs(LWP::Math::Vector3 value) {
		LWP::Math::Vector3 result{
			std::fabsf(value.x),
			std::fabsf(value.y),
			std::fabsf(value.z)
		};
		return result;
	}
}