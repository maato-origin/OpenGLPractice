#pragma once
#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	//このメッシュコンポーネントを指定のシェーダーで描画する
	virtual void Draw(class Shader* shader);
	//メッシュコンポーネントが使うmesh/textureインデックスの設定
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

private:
	class Mesh* mMesh;
	size_t mTextureIndex;
};