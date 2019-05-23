#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder)
	:MoveComponent(owner,updateOrder)
	,mNextNode(nullptr)
{

}

void NavComponent::Update(float deltaTime)
{
	//�������̓_�ɓ��B������A�o�H�̎��̓_�ɐi��
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
	//��������pos�ւ̃x�N�g��
	Vector2 dir = pos - mOwner->GetPosition();
	//�V�����p�x�́Adir�x�N�g����atan2
	//(��ʂł�+y�����Ȃ̂�y�̐��������]����)
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}