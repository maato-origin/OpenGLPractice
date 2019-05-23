#pragma once
#include "SDL/SDL.h"


struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	//�Q�[����������
	bool Initialize();
	//�Q�[�����[�v
	void RunLoop();
	//�Q�[���V���b�g�_�E��
	void Shutdown();

private:
	//�Q�[�����[�v�p�w���p�[�֐�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//SDL�����E�B���h�E
	SDL_Window* mWindow;
	//2D�`��p�����_���[
	SDL_Renderer* mRenderer;
	//�Q�[���J�n����̎���
	Uint32 mTicksCount;
	//�Q�[�����s���w������
	bool mIsRunning;

	//Pong�̎d�l
	//�p�h���̌���
	int mPaddleDir;
	//�p�h���̈ʒu
	Vector2 mPaddlePos;
	//�{�[���̈ʒu
	Vector2 mBallPos;
	//�{�[���̑��x
	Vector2 mBallVel;
};