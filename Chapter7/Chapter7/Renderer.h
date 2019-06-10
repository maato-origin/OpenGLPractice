#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Math.h"

struct DirectionalLight
{
	//光の方向
	Vector3 mDirection;
	//拡散反射色
	Vector3 mDiffuseColor;
	//鏡面反射色
	Vector3 mSpecColor;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();
	//レンダラーの初期化と修了処理
	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	//全てのテクスチャ・メッシュを開放
	void UnloadData();
	//フレームの描画
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

	//Mapでテクスチャを読み込み
	std::unordered_map<std::string, class Texture*> mTextures;
	//Mapでメッシュを読み込み
	std::unordered_map<std::string, class Mesh*> mMeshes;

	//全てのスプライトコンポーネントを描画
	std::vector<class SpriteComponent*> mSprites;

	//全てのメッシュコンポーネントを描画
	std::vector<class MeshComponent*> mMeshComps;

	//Game
	class Game* mGame;

	//スプライトシェーダー
	class Shader* mSpriteShader;
	//スプライト頂点配列
	class VertexArray* mSpriteVerts;

	//メッシュシェーダー
	class Shader* mMeshShader;

	//3Dシェーダー用のビュー射影行列
	Matrix4 mView;
	Matrix4 mProjection;
	//画面の幅と高さ
	float mScreenWidth;
	float mScreenHeight;

	//光源データ
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	//Window
	SDL_Window* mWindow;
	//OpenGLコンテキスト
	SDL_GLContext mContext;
};