#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddle1Dir(0)
,mPaddle2Dir(0)
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

	mPaddle1Pos.x = 10.0f;
	mPaddle1Pos.y = 768.0f / 2.0f;
	mPaddle2Pos.x = 999.0f;
	mPaddle2Pos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

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

	//パドル1の向きをW/Sキーで更新
	mPaddle1Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddle1Dir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddle1Dir += 1;
	}

	//パドル2の向きをI/Kキーで更新
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddle2Dir += 1;
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
	if (mPaddle1Dir != 0)
	{
		mPaddle1Pos.y += mPaddle1Dir * 300.0f * deltaTime;

		//パドルを画面外に出ないようにする
		if (mPaddle1Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle1Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle1Pos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddle1Pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 300.0f*deltaTime;

		//パドルを画面外に出ないようにする
		if (mPaddle2Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle2Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddle2Pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	//ボールの位置をボールの速度を基に更新
	mBallPos.x += mBallVel.x*deltaTime;
	mBallPos.y += mBallVel.y*deltaTime;

	//反射が求められる時
	//パドル1と接触しているか？
	float diff1 = mPaddle1Pos.y - mBallPos.y;
	//差分の絶対値を取る
	diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
	if (
		//yの差分が小さすぎるとき
		diff1 <= paddleH / 2.0f &&
		//xの位置が正常の時
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		//ボールが左へ動いているとき
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	//ボールはスクリーンの外へ出たか？
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	//パドル2
	float diff2 = mPaddle2Pos.y - mBallPos.y;
	//差分の絶対値を取る
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
	if (
		//yの差分が小さすぎるとき
		diff2 <= paddleH / 2.0f &&
		//xの位置が正常の時
		mBallPos.x <= 1004.0f && mBallPos.x >= 999.0f &&
		//ボールが右へ動いているとき
		mBallVel.x > 0.0f
		)
	{
		mBallVel.x *= -1.0f;
	}
	//ボールはスクリーンの外へ出たか
	else if (mBallPos.x >= 1024.0f)
	{
		mIsRunning = false;
	}

	//ボールは上の壁へ接触したか？
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	//ボールは下の壁へ接触したか？
	else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
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

	//ボールとパドルの色を描画
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	//2つのパドルを描画
	SDL_Rect paddle1{
		static_cast<int>(mPaddle1Pos.x),
		static_cast<int>(mPaddle1Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_Rect paddle2{
		static_cast<int>(mPaddle2Pos.x),
		static_cast<int>(mPaddle2Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle1);
	SDL_RenderFillRect(mRenderer, &paddle2);

	//ボールを描画
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	//フロントバッファとバックバッファを交換
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}