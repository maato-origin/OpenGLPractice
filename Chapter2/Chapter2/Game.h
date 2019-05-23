#pragma once
#include "SDL/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

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

	SDL_Texture* GetTexture(const std::string& fileName);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	//テクスチャのマップを読み込み
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	//ゲーム内の全てのアクター
	std::vector<class Actor*> mActors;
	//待ち状態のアクター
	std::vector<class Actor*> mPendingActors;

	//描画されたすべてのスプライトコンポーネント
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	//アクターを更新中か追跡
	bool mUpdatinActors;

	//ゲーム仕様
	class Ship* mShip;	//プレイヤーの船
};