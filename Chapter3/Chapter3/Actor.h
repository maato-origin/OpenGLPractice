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
		Edead
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
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

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
	Vector2 mPosition;	//�A�N�^�[�̒��S�ʒu
	float mScale;			//�A�N�^�[�̃X�P�[��(1.0f��100��)
	float mRotation;		//��]�̊p�x(���W�A��)

	//�A�N�^�[�����R���|�[�l���g
	std::vector<class Component*> mComponents;
	class Game* mGame;
};