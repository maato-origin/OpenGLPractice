#pragma once
#include "Component.h"
#include "SDL/SDL.h"

class SpriteComponent :public Component
{
public:
	//描画順序(drawOrder)が低いほど遠くに置かれる
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();
	virtual void Draw(SDL_Renderer* renderder);
	virtual void SetTexture(SDL_Texture* texture);
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

protected:
	//描画するテクスチャ
	SDL_Texture* mTexture;
	//画家のアルゴリズムで使う描画順序
	int mDrawOrder;
	//テクスチャの幅と高さ
	int mTexWidth;
	int mTexHeight;
};