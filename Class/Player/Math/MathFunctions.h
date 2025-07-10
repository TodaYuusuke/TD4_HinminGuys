#pragma once
#include <Adapter.h>

namespace MathFunc {
	/// <summary>
	/// 方向ベクトルからクォータニオンを算出
	/// </summary>
	/// <param name="dirVec"></param>
	/// <returns></returns>
	LWP::Math::Quaternion LookRotation(const LWP::Math::Vector3& dirVec);

	/// <summary>
	/// 地面と水平方向のビルボード
	/// </summary>
	/// <param name="cameraPos"></param>
	/// <param name="targetPos"></param>
	/// <returns></returns>
	LWP::Math::Quaternion HorizontalBillboard(const LWP::Math::Vector3& cameraPos, const LWP::Math::Vector3& targetPos);
	/// <summary>
	/// 速度によって大きさを変更するビルボード(使用不可)
	/// </summary>
	/// <param name="targetPos"></param>
	/// <param name="vel"></param>
	/// <param name="cameraPos"></param>
	/// <returns></returns>
	LWP::Math::Quaternion StretchedBillboard(const LWP::Math::Vector3& targetPos, const LWP::Math::Vector3& vel, const LWP::Math::Vector3& cameraPos);
	/// <summary>
	/// 行列からクォータニオンを抜き出す
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	//LWP::Math::Quaternion QuaternionRotateMatrix(const LWP::Math::Matrix4x4& m);
	/// <summary>
	/// 指数関数的に数値を増減させる補間
	/// </summary>
	/// <param name="current"></param>
	/// <param name="target"></param>
	/// <param name="damping"></param>
	/// <returns></returns>
	float ExponentialInterpolateF(const float& current, const float& target, float damping);
	/// <summary>
	/// 行列からオイラー角を抜き出す
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	float GetRotationAngleFromMatrix(const LWP::Math::Matrix4x4& m);
	/// <summary>
	/// 行列からクォータニオンを抜き出す
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	LWP::Math::Quaternion QuaternionFromMatrix(const LWP::Math::Matrix4x4& m);
}