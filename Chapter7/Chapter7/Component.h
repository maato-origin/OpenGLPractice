#pragma once
#include <cstdint>

class Component
{
public:
	//コンストラクター
	//updateOrderが小さいコンポーネントほど早く更新される
	Component(class Actor* owner, int updateOrder = 100);
	//デストラクター
	virtual ~Component();
	//このコンポーネントをデルタタイムで更新する
	virtual void Update(float deltaTime);
	//このコンポーネントのProcessInput
	virtual void ProcessInput(const uint8_t* keyState){}
	//ワールド変換更新時に呼び出す
	virtual void OnUpdateWorldTransform(){ }

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	//所有アクター
	class Actor* mOwner;
	//コンポーネントの更新順序
	int mUpdateOrder;
};