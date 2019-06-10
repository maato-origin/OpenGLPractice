#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mTexture(nullptr)
	,mDrawOrder(drawOrder)
	,mTexWidth(0)
	,mTexHeight(0)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture)
	{
		//�e�N�X�`���̕��ƍ����ŋ�`���X�P�[�����O
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f
		);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		//���[���h�ϊ��̐ݒ�
		shader->SetMatrixUniform("uWorldTransform", world);

		//���݃A�N�e�B�u�ȃe�N�X�`����ݒ�
		mTexture->SetActive();

		//�l�p�`��`��
		glDrawElements(
			GL_TRIANGLES,		//�`�悷��|���S��/�v���~�e�B�u�̎��
			6,					//�C���f�b�N�X�o�b�t�@�ɂ���C���f�b�N�X�̐�
			GL_UNSIGNED_INT,	//�C���f�b�N�X�̌^
			nullptr				//�ʏ��nullptr
		);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// ���ƍ����̐ݒ�
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}