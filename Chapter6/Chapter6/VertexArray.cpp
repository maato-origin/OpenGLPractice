#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	,mNumIndices(numIndices)
{
	//���_�z����쐬
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	//���_�o�b�t�@���쐬
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,				//�o�b�t�@�̎��
		numVerts * 8 * sizeof(float),	//�R�s�[����o�C�g��
		verts,							//�R�s�[��(�|�C���^)
		GL_STATIC_DRAW					//���̃f�[�^�̗��p���@
	);

	//�C���f�b�N�X�o�b�t�@���쐬
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			//�C���f�b�N�X�o�b�t�@�̎w��
		numIndices * sizeof(unsigned int),	//�f�[�^�̃T�C�Y
		indices, GL_STATIC_DRAW);

	//�ʒu���W�F3��float
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					//�����C���f�b�N�X(1�ڂ�0)
		3,					//�v�f��
		GL_FLOAT,			//�v�f�̌^
		GL_FALSE,			//(�����^�̂ݎg�p����)
		sizeof(float) * 8,	//�X�g���C�h(�ʏ�͊e���_�̃T�C�Y)
		0					//���_�f�[�^�̊J�n�ʒu���炱�̑����܂ł̃I�t�Z�b�g
	);
	//�@���x�N�g���F3��float
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					//���_�����C���f�b�N�X
		3,					//�����̐�(UV)
		GL_FLOAT,			//�e�����̌^
		GL_FALSE,			//(GL_FLOAT�ɂ͖��g�p)
		sizeof(float) * 8,	//�X�g���C�h
		reinterpret_cast<void*>(sizeof(float) * 3)	//�I�t�Z�b�g�|�C���^
	);
	//�e�N�X�`�����W�F2��float
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 8,
		reinterpret_cast<void*>(sizeof(float) * 6)
	);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}