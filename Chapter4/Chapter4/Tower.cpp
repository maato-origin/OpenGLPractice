#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"

Tower::Tower(class Game* game)
	:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));

	mMove = new MoveComponent(this);
	//mMove->SetAngularSpeed(Math::Pi);

	mNextAttack = AttackTime;
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mNextAttack -= deltaTime;
	if (mNextAttack <= 0.0f)
	{
		Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
		if (e != nullptr)
		{
			//���g����G�ւ̃x�N�g��
			Vector2 dir = e->GetPosition() - GetPosition();
			float dist = dir.Length();
			if (dist < AttackRange)
			{
				//�G�̕����ɉ�]
				SetRotation(Math::Atan2(-dir.y, dir.x));
				//�^���[�̈ʒu����G�Ɍ������Ēe�𐶐�
				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
		}
		mNextAttack += AttackTime;
	}
}