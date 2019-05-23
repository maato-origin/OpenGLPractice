#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddleDir(0)
{

}

bool Game::Initialize()
{
	//SDLの初期化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("SDLを初期化できません:%s", SDL_GetError());
		return false;
	}

	//SDLウィンドウを作成
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter1)",	//ウィンドウタイトル
		100,	//ウィンドウ左上隅のx座標
		100,	//ウィンドウ左上隅のy座標
		1024,	//ウィンドウの幅(width)
		768,	//ウィンドウの高さ(height)
		0		//フラグ（設定しない場合は0）
	);

	if (!mWindow)
	{
		SDL_Log("ウィンドウの作成に失敗:%s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,	//作成するレンダラーの描画対象となるウィンドウ
		-1,			//通常は-1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("レンダラーの作成に失敗:%s", SDL_GetError());
		return false;
	}

	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mball.push_back({ 1024.0f / 2.0f,768.0f / 2.0f,-200.0f,235.0f });
	mball.push_back({ 1024.0f / 2.0f,868.0f / 2.0f, 200.0f,235.0f });

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;

	//キューにイベントがあれば繰り返す
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			//ここで各種イベントを処理する
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	//キーボードの状態を取得
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//[Esc]キー入力でループを終了
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	//パドルの向きをW/Sキーで更新
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}
}

void Game::UpdateGame()
{
	//前のフレームから16msが経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	//deltaTimeは前のフレームとの時刻差を秒に変換した値
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//デルタタイムを最大値で制限する
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	//時刻を更新
	mTicksCount = SDL_GetTicks();

	//To do:ゲームワールドのオブジェクトを
	//デルタタイムの関数として更新する
	// ...

	//パドルの位置を移動方向を基に更新
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

		//パドルを画面外に出ないようにする
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	//ボールの位置をボールの速度を基に更新
	for (int i = 0; i < mball.size(); i++)
	{
		mball[i].pos.x += mball[i].vel.x*deltaTime;
		mball[i].pos.y += mball[i].vel.y*deltaTime;

		//反射が求められる時
		//パドルと接触しているか？
		float diff = mPaddlePos.y - mball[i].pos.y;
		//差分の絶対値を取る
		diff = (diff > 0.0f) ? diff : -diff;
		if (
			//yの差分が小さすぎるとき
			diff <= paddleH / 2.0f &&
			//xの位置が正常の時
			mball[i].pos.x <= 25.0f && mball[i].pos.x >= 20.0f &&
			//ボールが左へ動いているとき
			mball[i].vel.x < 0.0f)
		{
			mball[i].vel.x *= -1.0f;
		}
		//ボールはスクリーンの外へ出たか？
		else if (mball[i].pos.x <= 0.0f)
		{
			mIsRunning = false;
		}
		//ボールは右の壁へ接触したか？
		else if (mball[i].pos.x >= (1024.0f - thickness) && mball[i].vel.x > 0.0f)
		{
			mball[i].vel.x *= -1.0f;
		}

		//ボールは上の壁へ接触したか？
		if (mball[i].pos.y <= thickness && mball[i].vel.y < 0.0f)
		{
			mball[i].vel.y *= -1;
		}
		//ボールは下の壁へ接触したか？
		else if (mball[i].pos.y >= (768 - thickness) && mball[i].vel.y > 0.0f)
		{
			mball[i].vel.y *= -1;
		}
	}

}

void Game::GenerateOutput()
{
	//描画色を青に指定する
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		//R
		0,		//G
		255,	//B
		255		//A
	);

	//バックバッファをクリア
	SDL_RenderClear(mRenderer);

	//壁の色を描画
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);

	//上の壁を描画
	SDL_Rect wall{
		0,			//左上隅のx
		0,			//左上隅のy
		1024,		//幅
		thickness	//高さ
	};
	SDL_RenderFillRect(mRenderer, &wall);

	//下の壁を描画
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	//右の壁を描画
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	//ボールとパドルの色を描画
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	//パドルを描画
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);


	for (int i = 0; i < mball.size(); i++) 
	{
		//ボールを描画
		SDL_Rect ball{
			static_cast<int>(mball[i].pos.x - thickness / 2),
			static_cast<int>(mball[i].pos.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ball);
	}

	//フロントバッファとバックバッファを交換
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}