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

	//バンクのロード・アンロード
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void Update(float deltaTime);

	//ポジショナルオーディオ用
	void SetListener(const Matrix4& viewMatrix);
	//バスの制御
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);

protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	//イベントインスタンスで使うために次のIDを追跡する
	static unsigned int sNextID;

	class Game* mGame;
	//ロードされたバンクの連想配列
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	//イベント名からEventDescriptionへの連想配列
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	//イベントIDからEventInstanceへの連想配列
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	//バスの連想配列
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
	//FMOD studioシステム
	FMOD::Studio::System* mSystem;
	//FMOD Low-levelシステム
	FMOD::System* mLowLevelSystem;
};