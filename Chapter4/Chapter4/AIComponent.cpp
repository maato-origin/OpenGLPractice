#include "AIComponent.h"
#include "Actor.h"
#include "AIState.h"
#include <SDL/SDL_log.h>

AIComponent::AIComponent(class Actor* owner)
	:Component(owner)
	, mCurrentState(nullptr)
{

}

void AIComponent::Update(float deltaTime)
{
	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void AIComponent::ChangeState(const std::string& name)
{
	//�܂����݂̏�Ԃ𔲂���
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	//�V������Ԃ�A�z�z�񂩂�T��
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		//�V������Ԃɓ���
		mCurrentState->OnEnter();
	}
	else
	{
		SDL_Log("AIState %s �̏�Ԃ͂���܂���", name.c_str());
		mCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state)
{
	mStateMap.emplace(state->GetName(), state);
}