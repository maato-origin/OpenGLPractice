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
	//このテクスチャのOpenGL ID
	unsigned int mTextureID;
	//テクスチャの幅と高さ
	int mWidth;
	int mHeight;
};