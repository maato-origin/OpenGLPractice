#pragma once

class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	//この頂点配列をアクティブにする（描画できるようにする）
	void SetActive();

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	//頂点バッファにある頂点の数
	unsigned int mNumVerts;
	//インデックスバッファにあるインデックスの数
	unsigned int mNumIndices;
	//頂点バッファのOpenGL ID
	unsigned int mVertexBuffer;
	//インデックスバッファのOpenGL ID
	unsigned int mIndexBuffer;
	//頂点配列オブジェクトのOpenGL ID
	unsigned int mVertexArray;
};