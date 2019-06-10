#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include <GL/glew.h>

Renderer::Renderer(Game* game)
	:mGame(game)
	, mSpriteShader(nullptr)
	, mMeshShader(nullptr)
{

}

Renderer::~Renderer()
{

}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	//OpenGL�E�B���h�E�̑�����ݒ�
	//�R�AOpenGL�v���t�@�C�����g��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//�o�[�W����3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA�e�`���l��8�r�b�g�̃J���[�o�b�t�@���g�p
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//�_�u���o�b�t�@��L����
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//�n�[�h�E�F�A�A�N�Z�����[�V�������g�p
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter6)", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
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

	return true;
}

void Renderer::Shutdown()
{
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	//�e�N�X�`���̏���
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	//���b�V���̏���
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	//�N���A�J���[����F�ɐݒ�
	glClearColor(0.63f, 0.85f, 0.86f, 0.94f);
	//�J���[�o�b�t�@���N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//���b�V���R���|�[�l���g��`��
	//�[�x�o�b�t�@�̗L�����E�A���t�@�u�����h�𖳌���
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//���b�V���V�F�[�_�[���A�N�e�B�u�ɂ���
	mMeshShader->SetActive();
	//�r���[�ˉe�s����X�V
	mMeshShader->SetMatrixUniform("uViewProj", mView*mProjection);
	//�ψ�Ɩ��̍X�V
	SetLightUniforms(mMeshShader);
	for (auto mc : mMeshComps)
	{
		mc->Draw(mMeshShader);
	}

	//�S�ẴX�v���C�g�R���|�[�l���g��`��
	//�[�x�o�b�t�@�𖳌���
	glDisable(GL_DEPTH_TEST);
	//�J���[�o�b�t�@��̃A���t�@�u�����h��L����
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	//�X�v���C�g�̃V�F�[�_�[�ƒ��_�z��I�u�W�F�N�g���A�N�e�B�u��
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	//�S�ẴX�v���C�g��`��
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	//�o�b�t�@������
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
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

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	//�I�[�_�[���j�]����̂Ō����͂ł��Ȃ�
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
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

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

bool Renderer::LoadShaders()
{
	//�X�v���C�g�V�F�[�_�[�̍쐬
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();

	//�r���[�ˉe�s��̐ݒ�
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	//��{���b�V���V�F�[�_�[�̍쐬
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();

	//�r���[�Ǝˉe�s��̐ݒ�
	mView = Matrix4::CreateLookAt(
		Vector3::Zero,		//�J�����̈ʒu
		Vector3::UnitX,		//�^�[�Q�b�g�̈ʒu
		Vector3::UnitZ		//�����
	);
	mProjection = Matrix4::CreatePerspectiveFOV(
		Math::ToRadians(70.0f),		//��������
		mScreenWidth,				//�r���[�̕�
		mScreenHeight,				//�r���[�̍���
		25.0f,						//�ߐڕ��ʂ܂ł̋���
		10000.0f					//�������ʂ܂ł̋���
	);
	mMeshShader->SetMatrixUniform("uViewProj", mView*mProjection);

	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,	//����
		 0.5f,  0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,	//�E��
		 0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f,	//�E��
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f	//����
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader)
{
	//�J�����̈ʒu�̓r���[�𔽓]���ċ��߂�
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	//����
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	//���s����
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}