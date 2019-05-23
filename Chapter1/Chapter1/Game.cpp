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
	//SDL�̏�����
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("SDL���������ł��܂���:%s", SDL_GetError());
		return false;
	}

	//SDL�E�B���h�E���쐬
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter1)",	//�E�B���h�E�^�C�g��
		100,	//�E�B���h�E�������x���W
		100,	//�E�B���h�E�������y���W
		1024,	//�E�B���h�E�̕�(width)
		768,	//�E�B���h�E�̍���(height)
		0		//�t���O�i�ݒ肵�Ȃ��ꍇ��0�j
	);

	if (!mWindow)
	{
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s:%s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,	//�쐬���郌���_���[�̕`��ΏۂƂȂ�E�B���h�E
		-1,			//�ʏ��-1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("�����_���[�̍쐬�Ɏ��s:%s", SDL_GetError());
		return false;
	}

	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
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

	//�L���[�ɃC�x���g������ΌJ��Ԃ�
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			//�����Ŋe��C�x���g����������
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	//�L�[�{�[�h�̏�Ԃ��擾
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//[Esc]�L�[���͂Ń��[�v���I��
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	//�p�h���̌�����W/S�L�[�ōX�V
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
	//�O�̃t���[������16ms���o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	//deltaTime�͑O�̃t���[���Ƃ̎�������b�ɕϊ������l
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//�f���^�^�C�����ő�l�Ő�������
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	//�������X�V
	mTicksCount = SDL_GetTicks();

	//To do:�Q�[�����[���h�̃I�u�W�F�N�g��
	//�f���^�^�C���̊֐��Ƃ��čX�V����
	// ...

	//�p�h���̈ʒu���ړ���������ɍX�V
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

		//�p�h������ʊO�ɏo�Ȃ��悤�ɂ���
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	//�{�[���̈ʒu���{�[���̑��x����ɍX�V
	mBallPos.x += mBallVel.x*deltaTime;
	mBallPos.y += mBallVel.y*deltaTime;

	//���˂����߂��鎞
	//�p�h���ƐڐG���Ă��邩�H
	float diff = mPaddlePos.y - mBallPos.y;
	//�����̐�Βl�����
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		//y�̍���������������Ƃ�
		diff <= paddleH / 2.0f &&
		//x�̈ʒu������̎�
		mBallPos.x <= 25.0f&&mBallPos.x >= 20.0f &&
		//�{�[�������֓����Ă���Ƃ�
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	//�{�[���̓X�N���[���̊O�֏o�����H
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	//�{�[���͉E�̕ǂ֐ڐG�������H
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	//�{�[���͏�̕ǂ֐ڐG�������H
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	//�{�[���͉��̕ǂ֐ڐG�������H
	else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
}

void Game::GenerateOutput()
{
	//�`��F��Ɏw�肷��
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		//R
		0,		//G
		255,	//B
		255		//A
	);

	//�o�b�N�o�b�t�@���N���A
	SDL_RenderClear(mRenderer);

	//�ǂ̐F��`��
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);

	//��̕ǂ�`��
	SDL_Rect wall{
		0,			//�������x
		0,			//�������y
		1024,		//��
		thickness	//����
	};
	SDL_RenderFillRect(mRenderer, &wall);

	//���̕ǂ�`��
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	//�E�̕ǂ�`��
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	//�{�[���ƃp�h���̐F��`��
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	//�p�h����`��
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	//�{�[����`��
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	//�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@������
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}