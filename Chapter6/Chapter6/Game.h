#pragma once
#include "SDL/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	class Renderer* GetRenderer() { return mRenderer; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	//ゲーム内の全てのアクター
	std::vector<class Actor*> mActors;
	//待ち状態のアクター
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;
	//アクターを更新中か追跡
	bool mUpdatingActors;

	//ゲーム仕様
	class CameraActor* mCameraActor;
};