#include "Shader.h"
#include "Texture.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	:mShaderProgram(0)
	,mVertexShader(0)
	,mFragShader(0)
{
	
}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	//���_�V�F�[�_�[�ƃt���O�����g�V�F�[�_�[���R���p�C������
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	//���_/�t���O�����g�V�F�[�_�[�������N���ăV�F�[�_�[�v���O�������쐬
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);
	//�v���O�����������������N���ꂽ���Ƃ��m�F
	if (!IsValidProgram())
	{
		return false;
	}
	return true;
}

void Shader::Unload()
{
	//�v���O�����ƃV�F�[�_�[������
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	//�V�F�[�_�[�v���O�������A�N�e�B�u�ɂ���
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	//���̖��O��uniform��T��
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//�s��f�[�^��uniform�ɑ���
	glUniformMatrix4fv(
		loc,					//Uniform ID
		1,						//�s��̐�
		GL_TRUE,				//�s�x�N�g�����g���̂Ȃ�true
		matrix.GetAsFloatPtr()	//�s��f�[�^�ւ̃|�C���^
	);
}

bool Shader::CompileShader(const std::string& fileName,
	GLenum shaderType,
	GLuint& outShader)
{
	//�t�@�C�����J��
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		//�S�Ẵe�L�X�g��1�̕�����ɓǂݍ���
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		//�w�肳�ꂽ�^�C�v�̃V�F�[�_�[���쐬
		outShader = glCreateShader(shaderType);
		//�ǂݍ��񂾕�����ł̃R���p�C�������݂�
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	//�R���p�C������₢���킹��
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}
	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	//�����N�̏�Ԃ�₢���킹��
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}