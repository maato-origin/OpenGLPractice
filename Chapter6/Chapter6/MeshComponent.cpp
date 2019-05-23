#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
	,mMesh(nullptr)
	,mTextureIndex(0)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		//ワールド座標変換の設定
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		//スペキュラーの度合いを設定
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		//アクティブテクスチャの設定
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		//メッシュの頂点配列をアクティブにする
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		//描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}