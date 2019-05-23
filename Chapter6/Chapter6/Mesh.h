#pragma once
#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	//���b�V����Load/Unload
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	//���̃��b�V���Ɋ��蓖�Ă�ꂽ���_�z����擾
	class VertexArray* GetVertexArray() { return mVertexArray; }
	//�C���f�b�N�X����e�N�X�`�����擾
	class Texture* GetTexture(size_t index);
	//�V�F�[�_�[�����擾
	const std::string& GetShaderName() const { return mShaderName; }
	//�I�u�W�F�N�g��Ԃł̋��E���̔��a���擾
	float GetRadius() const { return mRadius; }
	//���b�V���̃X�y�L�����[�̓x�������擾
	float GetSpecPower() const { return mSpecPower; }

private:
	//���b�V���̃e�N�X�`���Q
	std::vector<class Texture*> mTextures;
	//���b�V���̒��_�z��
	class VertexArray* mVertexArray;
	//�V�F�[�_�[�̖��O
	std::string mShaderName;
	//�I�u�W�F�N�g��Ԃł̋��E���̔��a���L�^
	float mRadius;
	//�\�ʂ̃X�y�L�����[�̓x����
	float mSpecPower;
};