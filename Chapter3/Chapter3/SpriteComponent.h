#pragma once
#include "Component.h"
#include "SDL/SDL.h"

class SpriteComponent :public Component
{
public:
	//�`�揇��(drawOrder)���Ⴂ�قǉ����ɒu�����
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();
	virtual void Draw(SDL_Renderer* renderder);
	virtual void SetTexture(SDL_Texture* texture);
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

protected:
	//�`�悷��e�N�X�`��
	SDL_Texture* mTexture;
	//��Ƃ̃A���S���Y���Ŏg���`�揇��
	int mDrawOrder;
	//�e�N�X�`���̕��ƍ���
	int mTexWidth;
	int mTexHeight;
};