#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent :public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	//フレームごとにアニメーションを更新する(componentからオーバーライド)
	void Update(float deltaTime) override;
	//アニメーションに使うテクスチャを設定
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	//アニメーションのFPSを設定/取得
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	//アニメーションでのすべてのテクスチャ
	std::vector<SDL_Texture*> mAnimTextures;
	//現在表示しているフレーム
	float mCurrFrame;
	//アニメーションのフレームレート
	float mAnimFPS;
};