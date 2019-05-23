#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent :public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	//�t���[�����ƂɃA�j���[�V�������X�V����(component����I�[�o�[���C�h)
	void Update(float deltaTime) override;
	//�A�j���[�V�����Ɏg���e�N�X�`����ݒ�
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	//�A�j���[�V������FPS��ݒ�/�擾
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	//�A�j���[�V�����ł̂��ׂẴe�N�X�`��
	std::vector<SDL_Texture*> mAnimTextures;
	//���ݕ\�����Ă���t���[��
	float mCurrFrame;
	//�A�j���[�V�����̃t���[�����[�g
	float mAnimFPS;
};