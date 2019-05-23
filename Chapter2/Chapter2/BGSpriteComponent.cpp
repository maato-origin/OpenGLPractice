#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mScrollSpeed(0.0f)
{

}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	for (auto& bg : mBGTextures)
	{
		//x�̃I�t�Z�b�g���X�V
		bg.mOffset.x += mScrollSpeed * deltaTime;
		//������ʂ��犮�S�ɏo����I�t�Z�b�g��
		//�Ō�̔w�i�e�N�X�`���̉E�Ƀ��Z�b�g����
		if (bg.mOffset.x < -mScreenSize.x)
		{
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer)
{
	//���ꂼ��̔w�i�e�N�X�`����`��
	for (auto& bg : mBGTextures)
	{
		SDL_Rect r;
		//����2�����摜�T�C�Y
		r.w = static_cast<int>(mScreenSize.x);
		r.h = static_cast<int>(mScreenSize.y);
		//��`�̒��S�����L�A�N�^�[�̈ʒu�ɍ��킹��
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 + bg.mOffset.x);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 + bg.mOffset.y);

		//���̔w�i��`��
		SDL_RenderCopy(renderer,
			bg.mTexture,
			nullptr,
			&r
		);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mTexture = tex;
		//���ꂼ��̃e�N�X�`���͉�ʕ����̃I�t�Z�b�g������
		temp.mOffset.x = count * mScreenSize.x;
		temp.mOffset.y = 0;
		mBGTextures.emplace_back(temp);
		count++;
	}
}