#pragma once
#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	//メッシュのLoad/Unload
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	//このメッシュに割り当てられた頂点配列を取得
	class VertexArray* GetVertexArray() { return mVertexArray; }
	//インデックスからテクスチャを取得
	class Texture* GetTexture(size_t index);
	//シェーダー名を取得
	const std::string& GetShaderName() const { return mShaderName; }
	//オブジェクト空間での境界球の半径を取得
	float GetRadius() const { return mRadius; }
	//メッシュのスペキュラーの度合いを取得
	float GetSpecPower() const { return mSpecPower; }

private:
	//メッシュのテクスチャ群
	std::vector<class Texture*> mTextures;
	//メッシュの頂点配列
	class VertexArray* mVertexArray;
	//シェーダーの名前
	std::string mShaderName;
	//オブジェクト空間での境界球の半径を記録
	float mRadius;
	//表面のスペキュラーの度合い
	float mSpecPower;
};