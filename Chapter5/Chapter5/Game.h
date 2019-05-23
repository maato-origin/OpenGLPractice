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

	class Texture* GetTexture(const std::string& fileName);

	//ゲーム仕様(アステロイドの追加/削除)
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShaders();
	void CreateSpriteVerts();
	void LoadData();
	void UnloadData();

	//テクスチャのマップを読み込み
	std::unordered_map<std::string, class Texture*> mTextures;

	//ゲーム内の全てのアクター
	std::vector<class Actor*> mActors;
	//待ち状態のアクター
	std::vector<class Actor*> mPendingActors;

	//描画されたすべてのスプライトコンポーネント
	std::vector<class SpriteComponent*> mSprites;

	//スプライトシェーダー
	class Shader* mSpriteShader;
	//スプライト頂点配列
	class VertexArray* mSpriteVerts;

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	Uint32 mTicksCount;
	bool mIsRunning;
	//アクターを更新中か追跡
	bool mUpdatingActors;

	//ゲーム仕様
	class Ship* mShip;	//プレイヤーの船
	std::vector<class Asteroid*> mAsteroids;
};