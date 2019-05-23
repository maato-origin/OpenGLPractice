#pragma once
#include <random>
#include "Math.h"

class Random
{
public:
	static void Init();

	//特定の整数乱数生成器
	static void Seed(unsigned int seed);

	//0.0fから1.0f間の浮動小数点数を取得
	static float GetFloat();

	//特定範囲の浮動小数点数を取得
	static float GetFloatRange(float min, float max);

	//特定範囲の整数を取得
	static int GetIntRange(int min, int max);

	//min/max間のランダムのベクトルを取得
	static Vector2 GetVector(const Vector2& min, const Vector2& max);
	static Vector3 GetVector(const Vector3& min, const Vector3& max);

private:
	static std::mt19937 sGenerator;
};