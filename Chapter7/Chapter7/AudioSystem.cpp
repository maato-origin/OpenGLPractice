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
	//デバッグログの初期化
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR,	//エラーだけログを取る
		FMOD_DEBUG_MODE_TTY		//stdoutに出力
	);

	//FMOD studio システムオブジェクトの作成
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//FMOD studio システムの初期化
	result = mSystem->initialize(
		512,						//最大同時発音数
		FMOD_STUDIO_INIT_NORMAL,	//デフォルト設定
		FMOD_INIT_NORMAL,			//デフォルト設定
		nullptr						//通常はnullptr
	);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//Low Level システムポインタを取得して保存
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	//マスターバンクをロードする(stringが先)
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	//全てのバンクをアンロード
	UnloadAllBanks();
	//FMOD systemをシャットダウン
	if (mSystem)
	{
		mSystem->release();
	}
}

void AudioSystem::LoadBank(const std::string& name)
{
	//多重読み込みの防止
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	//バンクをロード
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),					//ファイル名
		FMOD_STUDIO_LOAD_BANK_NORMAL,	//通常の読み込み
		&bank							//バンクへのポインタを保存
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		//バンクを連想配列に追加
		mBanks.emplace(name, bank);
		//ストリーミング以外のサンプルデータを全てロード
		bank->loadSampleData();
		//このバンクにあるイベントの数を取得
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			//バンクにあるイベント記述子のリストを取得
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				//このイベントのパスを取得
				e->getPath(eventName, maxPathLength, nullptr);
				//イベント連想配列に追加
				mEvents.emplace(eventName, e);
			}
		}

		//バンク内にあるバスの数を取得
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			//バンク内にあるバスのリストを取得
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				//このバスのパスを取得
				bus->getPath(busName, 512, nullptr);
				//バスの連想配列に追加
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	//読み込まれていなければ無視
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	//最初にバンクからすべてのイベントを削除
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		//このバンク用のevent descriptionを取得
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		//イベントのリストを取得
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			//このイベントのパスを取得
			e->getPath(eventName, 512, nullptr);
			//このイベントを消去
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}

	//バンク内のバスの数を取得
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		//バンク内のバスのリストを取得
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			//バスのパスを取得
			bus->getPath(busName, 512, nullptr);
			//バスを消去
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end())
			{
				mBuses.erase(busi);
			}
		}
	}

	//サンプルデータとバンクをアンロード
	bank->unloadSampleData();
	bank->unload();
	//連想配列からバンクを消去
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		//サンプルデータとバンクをアンロード
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	//バンクがないことはイベントもないことを意味する
	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	//イベントの存在を確認
	unsigned int retID = 0;
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		//イベントのインスタンスを作成
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			//イベントインスタンスの開始
			event->start();
			//次のIDを取得し、連想配列に追加
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime)
{
	//ストップしたイベントインスタンスを探す
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		//イベントの状態を取得
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			//イベントを解放してidを終了リストに追加
			e->release();
			done.emplace_back(iter.first);
		}
	}

	//終了したイベントインスタンスを連想配列から削除
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	//FMODの更新
	mSystem->update();
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		//ゲーム座標をFMODに変換
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
	//ベクトルを得るためのビュー行列の逆行列を計算
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	//位置と方向をセット
	listener.position = VecToFMOD(invView.GetTranslation());
	//逆ビュー行列では第3行が前方向
	listener.forward = VecToFMOD(invView.GetZAxis());
	//逆ビュー行列では第2行が上方向
	listener.up = VecToFMOD(invView.GetYAxis());
	//速度を0に設定（ドップラー効果仕様の場合は修正）
	listener.velocity = { 0.0f,0.0f,0.0f };
	//FMODに送る(0はリスナーが一人)
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