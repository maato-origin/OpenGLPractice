#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{

}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		//�t���[�����[�g�ƃf���^�^�C���Ɋ�Â��ăJ�����g�t���[�����X�V
		mCurrFrame += mAnimFPS * deltaTime;

		//�K�v�ɉ����ăJ�����g�t���[���������߂�
		while (mCurrFrame >= mAnimTextures.size())
		{
			mCurrFrame -= mAnimTextures.size();
		}

		//�����_�̃e�N�X�`����ݒ肷��
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		//�ŏ��̃t���[���ɃA�N�e�B�u�ȃe�N�X�`����ݒ�
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}