#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	//updateOrder�̒l���������قǐ�ɍX�V�����
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
	//��]�𐧌�(���W�A��/�b)
	float mAngularSpeed;
	//�ړ��𐧌�(�P�ʒ�/�b)
	float mForwardSpeed;
};