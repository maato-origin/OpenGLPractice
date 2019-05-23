#pragma once
#include <random>
#include "Math.h"

class Random
{
public:
	static void Init();

	//����̐�������������
	static void Seed(unsigned int seed);

	//0.0f����1.0f�Ԃ̕��������_�����擾
	static float GetFloat();

	//����͈͂̕��������_�����擾
	static float GetFloatRange(float min, float max);

	//����͈͂̐������擾
	static int GetIntRange(int min, int max);

	//min/max�Ԃ̃����_���̃x�N�g�����擾
	static Vector2 GetVector(const Vector2& min, const Vector2& max);
	static Vector3 GetVector(const Vector3& min, const Vector3& max);

private:
	static std::mt19937 sGenerator;
};