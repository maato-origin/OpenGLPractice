#include "AudioSystem.h"
#include <SDL/SDL_log.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	:mGame(game)
	, mSystem(nullptr)
	, mLowLevelSystem(nullptr)
{

}

AudioSystem::~AudioSystem()
{

}

bool AudioSystem::Initialize()
{
	//�f�o�b�O���O�̏�����
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR,	//�G���[�������O�����
		FMOD_DEBUG_MODE_TTY		//stdout�ɏo��
	);

	//FMOD studio �V�X�e���I�u�W�F�N�g�̍쐬
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//FMOD studio �V�X�e���̏�����
	result = mSystem->initialize(
		512,						//�ő哯��������
		FMOD_STUDIO_INIT_NORMAL,	//�f�t�H���g�ݒ�
		FMOD_INIT_NORMAL,			//�f�t�H���g�ݒ�
		nullptr						//�ʏ��nullptr
	);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//Low Level �V�X�e���|�C���^���擾���ĕۑ�
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	//�}�X�^�[�o���N�����[�h����(string����)
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	//�S�Ẵo���N���A�����[�h
	UnloadAllBanks();
	//FMOD system���V���b�g�_�E��
	if (mSystem)
	{
		mSystem->release();
	}
}

void AudioSystem::LoadBank(const std::string& name)
{
	//���d�ǂݍ��݂̖h�~
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	//�o���N�����[�h
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),					//�t�@�C����
		FMOD_STUDIO_LOAD_BANK_NORMAL,	//�ʏ�̓ǂݍ���
		&bank							//�o���N�ւ̃|�C���^��ۑ�
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		//�o���N��A�z�z��ɒǉ�
		mBanks.emplace(name, bank);
		//�X�g���[�~���O�ȊO�̃T���v���f�[�^��S�ă��[�h
		bank->loadSampleData();
		//���̃o���N�ɂ���C�x���g�̐����擾
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			//�o���N�ɂ���C�x���g�L�q�q�̃��X�g���擾
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				//���̃C�x���g�̃p�X���擾
				e->getPath(eventName, maxPathLength, nullptr);
				//�C�x���g�A�z�z��ɒǉ�
				mEvents.emplace(eventName, e);
			}
		}

		//�o���N���ɂ���o�X�̐����擾
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			//�o���N���ɂ���o�X�̃��X�g���擾
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				//���̃o�X�̃p�X���擾
				bus->getPath(busName, 512, nullptr);
				//�o�X�̘A�z�z��ɒǉ�
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	//�ǂݍ��܂�Ă��Ȃ���Ζ���
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	//�ŏ��Ƀo���N���炷�ׂẴC�x���g���폜
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		//���̃o���N�p��event description���擾
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		//�C�x���g�̃��X�g���擾
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			//���̃C�x���g�̃p�X���擾
			e->getPath(eventName, 512, nullptr);
			//���̃C�x���g������
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}

	//�o���N���̃o�X�̐����擾
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		//�o���N���̃o�X�̃��X�g���擾
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			//�o�X�̃p�X���擾
			bus->getPath(busName, 512, nullptr);
			//�o�X������
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end())
			{
				mBuses.erase(busi);
			}
		}
	}

	//�T���v���f�[�^�ƃo���N���A�����[�h
	bank->unloadSampleData();
	bank->unload();
	//�A�z�z�񂩂�o���N������
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		//�T���v���f�[�^�ƃo���N���A�����[�h
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	//�o���N���Ȃ����Ƃ̓C�x���g���Ȃ����Ƃ��Ӗ�����
	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	//�C�x���g�̑��݂��m�F
	unsigned int retID = 0;
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		//�C�x���g�̃C���X�^���X���쐬
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			//�C�x���g�C���X�^���X�̊J�n
			event->start();
			//����ID���擾���A�A�z�z��ɒǉ�
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime)
{
	//�X�g�b�v�����C�x���g�C���X�^���X��T��
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		//�C�x���g�̏�Ԃ��擾
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			//�C�x���g���������id���I�����X�g�ɒǉ�
			e->release();
			done.emplace_back(iter.first);
		}
	}

	//�I�������C�x���g�C���X�^���X��A�z�z�񂩂�폜
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	//FMOD�̍X�V
	mSystem->update();
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		//�Q�[�����W��FMOD�ɕϊ�
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
	//�x�N�g���𓾂邽�߂̃r���[�s��̋t�s����v�Z
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	//�ʒu�ƕ������Z�b�g
	listener.position = VecToFMOD(invView.GetTranslation());
	//�t�r���[�s��ł͑�3�s���O����
	listener.forward = VecToFMOD(invView.GetZAxis());
	//�t�r���[�s��ł͑�2�s�������
	listener.up = VecToFMOD(invView.GetYAxis());
	//���x��0�ɐݒ�i�h�b�v���[���ʎd�l�̏ꍇ�͏C���j
	listener.velocity = { 0.0f,0.0f,0.0f };
	//FMOD�ɑ���(0�̓��X�i�[����l)
	mSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setPaused(pause);
	}
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end())
	{
		event = iter->second;
	}
	return event;
}