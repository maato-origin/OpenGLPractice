#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
	,mCircle(nullptr)
{
	//�����_���Ȉʒu/�����ŏ�����
	Vector2 randPos = Random::GetVector(Vector2(-512.0f,-384.0f),
		Vector2(512.0f, 384.0f));
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	//�X�v���C�g�R���|�[�l���g���쐬���A�e�N�X�`����ݒ�
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//�^���R���|�[�l���g���쐬���A�O�i���x��ݒ�
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);

	//CircleComponent���쐬(�R���W�����p)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	//mAsteroids��game�ɉ�����
	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}