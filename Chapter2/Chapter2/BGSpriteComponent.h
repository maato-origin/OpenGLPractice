#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

class BGSpriteComponent : public SpriteComponent
{
public:
	//�`�揇���̏����l�͉�����i�����炱���w�i�ƂȂ�j
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);
	//�X�V�ƕ`��͐e����I�[�o�[���C�h
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	//�w�i�p�e�N�X�`����ݒ�
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	//��ʃT�C�Y�ƃX�N���[�����x�̐ݒ�/�擾
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }

private:
	//�X�̔w�i�摜�ƃI�t�Z�b�g���J�v�Z��������\����
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};

	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};