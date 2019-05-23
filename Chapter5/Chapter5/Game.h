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

	//�Q�[���d�l(�A�X�e���C�h�̒ǉ�/�폜)
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

	//�e�N�X�`���̃}�b�v��ǂݍ���
	std::unordered_map<std::string, class Texture*> mTextures;

	//�Q�[�����̑S�ẴA�N�^�[
	std::vector<class Actor*> mActors;
	//�҂���Ԃ̃A�N�^�[
	std::vector<class Actor*> mPendingActors;

	//�`�悳�ꂽ���ׂẴX�v���C�g�R���|�[�l���g
	std::vector<class SpriteComponent*> mSprites;

	//�X�v���C�g�V�F�[�_�[
	class Shader* mSpriteShader;
	//�X�v���C�g���_�z��
	class VertexArray* mSpriteVerts;

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	Uint32 mTicksCount;
	bool mIsRunning;
	//�A�N�^�[���X�V�����ǐ�
	bool mUpdatingActors;

	//�Q�[���d�l
	class Ship* mShip;	//�v���C���[�̑D
	std::vector<class Asteroid*> mAsteroids;
};