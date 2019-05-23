#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Math.h"

struct DirectionalLight
{
	//���̕���
	Vector3 mDirection;
	//�g�U���ːF
	Vector3 mDiffuseColor;
	//���ʔ��ːF
	Vector3 mSpecColor;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();
	//�����_���[�̏������ƏC������
	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	//�S�Ẵe�N�X�`���E���b�V�����J��
	void UnloadData();
	//�t���[���̕`��
	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { mView = view; }

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return mDirLight; }

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	//Map�Ńe�N�X�`����ǂݍ���
	std::unordered_map<std::string, class Texture*> mTextures;
	//Map�Ń��b�V����ǂݍ���
	std::unordered_map<std::string, class Mesh*> mMeshes;

	//�S�ẴX�v���C�g�R���|�[�l���g��`��
	std::vector<class SpriteComponent*> mSprites;

	//�S�Ẵ��b�V���R���|�[�l���g��`��
	std::vector<class MeshComponent*> mMeshComps;

	//Game
	class Game* mGame;

	//�X�v���C�g�V�F�[�_�[
	class Shader* mSpriteShader;
	//�X�v���C�g���_�z��
	class VertexArray* mSpriteVerts;

	//���b�V���V�F�[�_�[
	class Shader* mMeshShader;

	//3D�V�F�[�_�[�p�̃r���[�ˉe�s��
	Matrix4 mView;
	Matrix4 mProjection;
	//��ʂ̕��ƍ���
	float mScreenWidth;
	float mScreenHeight;

	//�����f�[�^
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	//Window
	SDL_Window* mWindow;
	//OpenGL�R���e�L�X�g
	SDL_GLContext mContext;
};