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

	//�Q�[�����̑S�ẴA�N�^�[
	std::vector<class Actor*> mActors;
	//�҂���Ԃ̃A�N�^�[
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;
	//�A�N�^�[���X�V�����ǐ�
	bool mUpdatingActors;

	//�Q�[���d�l
	class CameraActor* mCameraActor;
};