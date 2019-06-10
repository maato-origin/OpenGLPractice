#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	//�R���X�g���N�^�ƃf�X�g���N�^
	Actor(class Game* game);
	virtual ~Actor();

	//�Q�[������Ăяo�����X�V�֐�(�I�[�o�[���C�h�s��)
	void Update(float deltaTime);
	//�A�N�^�[�����S�R���|�[�l���g���X�V(�I�[�o�[���C�h�s��)
	void UpdateComponents(float deltaTime);
	//�A�N�^�[�Ǝ��̍X�V����(�I�[�o�[���C�h�\)
	virtual void UpdateActor(float deltaTime);

	//ProcessInput�֐��̓Q�[������Ăяo�����i�I�[�o�[���C�h�s�j
	void ProcessInput(const uint8_t* keyState);
	//�A�N�^�[�Ǝ��̓��͗p�R�[�h�ŃI�[�o�[���C�h�\
	virtual void ActorInput(const uint8_t* keyState);

	//�Q�b�^�[/�Z�b�^�[
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	//�R���|�[�l���g�̒ǉ�/�폜
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	//�A�N�^�[�̏��
	State mState;
	//���W�ϊ�
	Matrix4 mWorldTransform;
	Vector3 mPosition;		//�A�N�^�[�̈ʒu
	Quaternion mRotation;	//��]
	float mScale;			//�A�N�^�[�̃X�P�[��(1.0f��100��)
	bool mRecomputeWorldTransform;

	//�A�N�^�[�����R���|�[�l���g
	std::vector<class Component*> mComponents;
	class Game* mGame;
};