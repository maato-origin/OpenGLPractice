#include "Game.h"
#include <GL/glew.h>
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"

Game::Game()
	:mWindow(nullptr)
	,mSpriteShader(nullptr)
	,mIsRunning(true)
	,mUpdatingActors(false)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDL�̏������Ɏ��s:%s", SDL_GetError());
		return false;
	}

	//OpenGL�E�B���h�E�̑�����ݒ�
	//�R�AOpenGL�v���t�@�C�����g��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//�o�[�W����3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA�e�`���l��8�r�b�g�̃J���[�o�b�t�@���g�p
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//�_�u���o�b�t�@��L����
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//�n�[�h�E�F�A�A�N�Z�����[�V�������g�p
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter5)", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s:%s", SDL_GetError());
		return false;
	}

	//OpenGL�̃R���e�N�X�g�𐶐�
	mContext = SDL_GL_CreateContext(mWindow);

	//GLEW��������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW�̏������Ɏ��s�D");
		return false;
	}

	//�ꕔ�v���b�g�t�H�[���Ŗ��Q�ȃG���[�R�[�h���o��
	//������N���A����
	glGetError();

	//�V�F�[�_�[�̍쐬/�R���p�C�����\���m�F����
	if (!LoadShaders())
	{
		SDL_Log("�V�F�[�_�[�̓ǂݍ��݂Ɏ��s�D");
		return false;
	}

	//�X�v���C�g�`��̂��߂Ɏl�p�`�𐶐�
	CreateSpriteVerts();

	LoadData();

	mTicksCount = SDL_GetTicks();

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
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame()
{
	//�f���^�^�C�����v�Z
	//�ŏI�t���[������16ms�o�߂܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	//�S�ẴA�N�^�[���X�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	//�҂��A�N�^�[��mActors�Ɉړ�
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	//�ꎟ�z��ɏ��ŃA�N�^�[��ǉ�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	//���ŃA�N�^�[�̍폜(mActors����폜)
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	//�N���A�J���[���D�F�ɐݒ�
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	//�J���[�o�b�t�@���N���A
	glClear(GL_COLOR_BUFFER_BIT);

	//�A���t�@�u�����h��L����
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,				//srcFactor��srcAlpha
		GL_ONE_MINUS_SRC_ALPHA		//dstFactor��(1-srcAlpha)
	);

	//�X�v���C�g�̃V�F�[�_�[�ƒ��_�z��I�u�W�F�N�g���A�N�e�B�u��
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	//�S�ẴX�v���C�g��`��
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	//�o�b�t�@�������B����ŃV�[�����\�������
	SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShaders()
{
	mSpriteShader = new Shader();
	if(!mSpriteShader->Load("Shaders/Sprite.vert","Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();

	//�r���[�ˉe�s��̐ݒ�
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	return true;
}

void Game::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f,	//����
		 0.5f,  0.5f, 0.f, 1.f, 0.f,	//�E��
		 0.5f, -0.5f, 0.f, 1.f, 1.f,	//�E��
		-0.5f, -0.5f, 0.f, 0.f, 1.f		//����
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData()
{
	//�v���C���[�̑D���쐬
	mShip = new Ship(this);
	mShip->SetRotation(Math::PiOver2);

	//�A�X�e���C�h�̍쐬
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	//�A�N�^�[�̏���
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	//�e�N�X�`���[�̏���
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
}

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	//�e�N�X�`�������łɘA�z�z��ɓ����Ă��邩�H
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) {
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}

	return tex;
}

void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}

void Game::Shutdown()
{
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	//�A�N�^�[�̍X�V���Ȃ�҂��ɒǉ�
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	//pending actors�̒��ɂ��邩
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//vector�̍Ō�̈ʒu�����������g�͍폜
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//actors�̒��ɂ��邩
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//vector�̍Ō�̈ʒu�����������g�͍폜
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	//�\�[�g�ς݂̔z��ő}���_��������
	//(������菇���̍����ŏ��̗v�f�̈ʒu)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	//�C�e���[�^�[�̈ʒu�̑O�ɗv�f��}������
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	//�I�[�_�[���j�]����̂Ō����͂ł��Ȃ�
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}