#pragma once
#include <cstdint>

class Component
{
public:
	//�R���X�g���N�^�[
	//updateOrder���������R���|�[�l���g�قǑ����X�V�����
	Component(class Actor* owner, int updateOrder = 100);
	//�f�X�g���N�^�[
	virtual ~Component();
	//���̃R���|�[�l���g���f���^�^�C���ōX�V����
	virtual void Update(float deltaTime);
	//���̃R���|�[�l���g��ProcessInput
	virtual void ProcessInput(const uint8_t* keyState){}
	//���[���h�ϊ��X�V���ɌĂяo��
	virtual void OnUpdateWorldTransform(){ }

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	//���L�A�N�^�[
	class Actor* mOwner;
	//�R���|�[�l���g�̍X�V����
	int mUpdateOrder;
};