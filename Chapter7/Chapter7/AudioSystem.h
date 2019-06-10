#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class AudioSystem
{
public:
	AudioSystem(class Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();

	//�o���N�̃��[�h�E�A�����[�h
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void Update(float deltaTime);

protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	//�C�x���g�C���X�^���X�Ŏg�����߂Ɏ���ID��ǐՂ���
	static unsigned int sNextID;

	class Game* mGame;
	//���[�h���ꂽ�o���N�̘A�z�z��
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	//�C�x���g������EventDescription�ւ̘A�z�z��
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	//�C�x���gID����EventInstance�ւ̘A�z�z��
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	//FMOD studio�V�X�e��
	FMOD::Studio::System* mSystem;
	//FMOD Low-level�V�X�e��
	FMOD::System* mLowLevelSystem;
};