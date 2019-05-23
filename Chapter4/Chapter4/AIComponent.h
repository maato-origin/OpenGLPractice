#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent : public Component
{
public:
	AIComponent(class Actor* owner);

	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);

	//�V���ȏ�Ԃ�A�z�z��ɓo�^
	void RegisterState(class AIState* state);

private:
	//��Ԃ̖��O��AIState�̃C���X�^���X��Ή��t����
	std::unordered_map<std::string, class AIState*> mStateMap;
	//���݂̏��
	class AIState* mCurrentState;
};