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
		//テクスチャの幅と高さで矩形をスケーリング
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f
		);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		//ワールド変換の設定
		shader->SetMatrixUniform("uWorldTransform", world);

		//現在アクティブなテクスチャを設定
		mTexture->SetActive();

		//四角形を描画
		glDrawElements(
			GL_TRIANGLES,		//描画するポリゴン/プリミティブの種類
			6,					//インデックスバッファにあるインデックスの数
			GL_UNSIGNED_INT,	//インデックスの型
			nullptr				//通常はnullptr
		);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// 幅と高さの設定
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}