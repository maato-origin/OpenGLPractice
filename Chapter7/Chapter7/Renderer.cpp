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

	//OpenGLウィンドウの属性を設定
	//コアOpenGLプロファイルを使う
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//バージョン3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA各チャネル8ビットのカラーバッファを使用
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//ダブルバッファを有効化
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//ハードウェアアクセラレーションを使用
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter6)", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("ウィンドウの作成に失敗:%s", SDL_GetError());
		return false;
	}

	//OpenGLのコンテクストを生成
	mContext = SDL_GL_CreateContext(mWindow);

	//GLEWを初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEWの初期化に失敗．");
		return false;
	}

	//一部プラットフォームで無害なエラーコードを出す
	//それをクリアする
	glGetError();

	//シェーダーの作成/コンパイルが可能か確認する
	if (!LoadShaders())
	{
		SDL_Log("シェーダーの読み込みに失敗．");
		return false;
	}

	//スプライト描画のために四角形を生成
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
	//テクスチャの消去
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	//メッシュの消去
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	//クリアカラーを空色に設定
	glClearColor(0.63f, 0.85f, 0.86f, 0.94f);
	//カラーバッファをクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//メッシュコンポーネントを描画
	//深度バッファの有効化・アルファブレンドを無効化
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//メッシュシェーダーをアクティブにする
	mMeshShader->SetActive();
	//ビュー射影行列を更新
	mMeshShader->SetMatrixUniform("uViewProj", mView*mProjection);
	//均一照明の更新
	SetLightUniforms(mMeshShader);
	for (auto mc : mMeshComps)
	{
		mc->Draw(mMeshShader);
	}

	//全てのスプライトコンポーネントを描画
	//深度バッファを無効化
	glDisable(GL_DEPTH_TEST);
	//カラーバッファ上のアルファブレンドを有効化
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	//スプライトのシェーダーと頂点配列オブジェクトをアクティブ化
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	//全てのスプライトを描画
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	//バッファを交換
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	//ソート済みの配列で挿入点を見つける
	//(自分より順序の高い最初の要素の位置)
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

	//イテレーターの位置の前に要素を挿入する
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	//オーダーが破綻するので交換はできない
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
	//テクスチャがすでに連想配列に入っているか？
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
	//スプライトシェーダーの作成
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();

	//ビュー射影行列の設定
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	//基本メッシュシェーダーの作成
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();

	//ビューと射影行列の設定
	mView = Matrix4::CreateLookAt(
		Vector3::Zero,		//カメラの位置
		Vector3::UnitX,		//ターゲットの位置
		Vector3::UnitZ		//上向き
	);
	mProjection = Matrix4::CreatePerspectiveFOV(
		Math::ToRadians(70.0f),		//水平視野
		mScreenWidth,				//ビューの幅
		mScreenHeight,				//ビューの高さ
		25.0f,						//近接平面までの距離
		10000.0f					//遠方平面までの距離
	);
	mMeshShader->SetMatrixUniform("uViewProj", mView*mProjection);

	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,	//左上
		 0.5f,  0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,	//右上
		 0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f,	//右下
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f	//左下
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader)
{
	//カメラの位置はビューを反転して求める
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	//環境光
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	//平行光源
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}