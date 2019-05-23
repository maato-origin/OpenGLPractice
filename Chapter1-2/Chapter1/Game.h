#pragma once
#include "SDL/SDL.h"
#include "vector"

struct Vector2
{
	float x;
	float y;
};
struct Ball
{
	//ボールの位置
	Vector2 pos;
	//ボールの速度
	Vector2 vel;
};

class Game
{
public:
	Game();
	//ゲームを初期化
	bool Initialize();
	//ゲームループ
	void RunLoop();
	//ゲームシャットダウン
	void Shutdown();

private:
	//ゲームループ用ヘルパー関数
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//SDLが作るウィンドウ
	SDL_Window* mWindow;
	//2D描画用レンダラー
	SDL_Renderer* mRenderer;
	//ゲーム開始からの時間
	Uint32 mTicksCount;
	//ゲーム続行を指示する
	bool mIsRunning;

	//Pongの仕様
	//パドルの向き
	int mPaddleDir;
	//パドルの位置
	Vector2 mPaddlePos;
	//ボールの定義
	std::vector<Ball> mball;

};