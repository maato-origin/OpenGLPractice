#pragma once
#include "MoveComponent.h"

class InputComponent : public MoveComponent
{
public:
	//updateOrder�̒l���������قǐ�ɍX�V�����
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;

	//private�ϐ��̎擾�Ɛݒ�
	float GetMaxForward() const { return mMaxForwardSpeed; }
	float GetMaxAngular() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

	void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }

private:
	//�O�i/��]�̍ő呬�x
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;
	//�O�i/��ނ̂��߂̃L�[
	int mForwardKey;
	int mBackKey;
	//��]�^���̂��߂̃L�[
	int mClockwiseKey;
	int mCounterClockwiseKey;
};