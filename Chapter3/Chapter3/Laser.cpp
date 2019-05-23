#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* game)
	:Actor(game)
	,mDeathTimer(1.0f)
{
	//�X�v���C�g�R���|�[�l���g���쐬
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("../Assets/Laser.png"));

	//MoveComponent���쐬���A�O�i���x��ݒ�
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.0f);

	//CircleComponent���쐬(�R���W�����p)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime)
{
	//���Ԑ؂�Ń��[�U�[������
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.0f)
	{
		SetState(Edead);
	}
	else
	{
		//���f���ƌ������邩�H
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				//���[�U�[�����f���ƌ�������Ȃ�
				//���[�U�[�����f���̏����ĂȂ��Ȃ�
				SetState(Edead);
				ast->SetState(Edead);
				break;
			}
		}
	}
}