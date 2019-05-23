#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder)
	:MoveComponent(owner,updateOrder)
	,mNextNode(nullptr)
{

}

void NavComponent::Update(float deltaTime)
{
	//もし次の点に到達したら、経路の次の点に進む
	Vector2 diff = mOwner->GetPosition() - mNextNode->GetPosition();
	if (Math::NearZero(diff.Length(), 2.0f))
	{
		mNextNode = mNextNode->GetParent();
		TurnTo(mNextNode->GetPosition());
	}

	MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* start)
{
	mNextNode = start->GetParent();
	TurnTo(mNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos)
{
	//自分からposへのベクトル
	Vector2 dir = pos - mOwner->GetPosition();
	//新しい角度は、dirベクトルのatan2
	//(画面では+yが下なのでyの正負が反転する)
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}