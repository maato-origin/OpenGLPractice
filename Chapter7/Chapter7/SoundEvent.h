#pragma once
#include <string>
#include "Math.h"

class SoundEvent
{
public:
	SoundEvent();
	//対応するFMODイベントインスタンスが存在している場合trueを返す
	bool IsValid();
	//イベントの再開
	void Restart();
	//イベントの停止
	void Stop(bool allowFadeOut = true);
	//セッター関数
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	//ゲッター関数
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);
	//ポジショナル
	bool Is3D() const;
	void Set3DAttributes(const Matrix4& worldTrans);
	
protected:
	//コンストラクターがprotectedでAudioSystemがfriendなので
	//AudioSystemだけがコンストラクターにアクセス可能
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	class AudioSystem* mSystem;
	unsigned int mID;
};