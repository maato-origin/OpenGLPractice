#include "Texture.h"
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <SDL/SDL.h>

Texture::Texture()
	:mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{

}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;

	unsigned char* image = SOIL_load_image(
		fileName.c_str(),	//ファイル名
		&mWidth,			//幅が記録される
		&mHeight,			//高さが記録される
		&channels,			//チャネル数が記録される
		SOIL_LOAD_AUTO		//画像ファイルの種類
	);

	if (image == nullptr)
	{
		SDL_Log("SOIL failed to laod image %s : %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(
		GL_TEXTURE_2D,			//テクスチャターゲット
		0,						//LoD(詳細レベル)
		format,					//OpenGLが使うべきカラーフォーマット
		mWidth,					//テクスチャの幅
		mHeight,				//テクスチャの高さ
		0,						//境界線
		format,					//入力データのカラーフォーマット
		GL_UNSIGNED_BYTE,		//unsigned byteで8ビット/チャネルを指定
		image					//画像データへのポインタ
	);

	SOIL_free_image_data(image);

	//バイリニアフィルタを有効にする
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}