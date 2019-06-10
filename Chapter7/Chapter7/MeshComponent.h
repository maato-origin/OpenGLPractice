#pragma once
#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	//���̃��b�V���R���|�[�l���g���w��̃V�F�[�_�[�ŕ`�悷��
	virtual void Draw(class Shader* shader);
	//���b�V���R���|�[�l���g���g��mesh/texture�C���f�b�N�X�̐ݒ�
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

private:
	class Mesh* mMesh;
	size_t mTextureIndex;
};