#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void SetActive();

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
private:
	//���̃e�N�X�`����OpenGL ID
	unsigned int mTextureID;
	//�e�N�X�`���̕��ƍ���
	int mWidth;
	int mHeight;
};