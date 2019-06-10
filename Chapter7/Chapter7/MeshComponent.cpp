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
		//���[���h���W�ϊ��̐ݒ�
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		//�X�y�L�����[�̓x������ݒ�
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		//�A�N�e�B�u�e�N�X�`���̐ݒ�
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		//���b�V���̒��_�z����A�N�e�B�u�ɂ���
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		//�`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}