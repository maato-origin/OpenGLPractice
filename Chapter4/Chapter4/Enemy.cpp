#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "NavComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "CircleComponent.h"
#include <algorithm>

Enemy::Enemy(class Game* game)
	:Actor(game)
{
	//�G�̔z���������
	game->GetEnemies().emplace_back(this);

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	//start�^�C���Ɉʒu��ݒ�
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
	//start�^�C����nav component��ݒ�
	NavComponent* nc = new NavComponent(this);
	nc->SetForwardSpeed(150.0f);
	nc->StartPath(GetGame()->GetGrid()->GetStartTile());
	//�~�`�R���W�����̐ݒ�
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(25.0f);
}

Enemy::~Enemy()
{
	//�G�̔z�񂩂�폜
	auto iter = std::find(GetGame()->GetEnemies().begin(),
		GetGame()->GetEnemies().end(),
		this);
	GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	//end�^�C���ɋ߂Â��Ă��邩
	Vector2 diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();
	if(Math::NearZero(diff.Length(),10.0f))
	{
		SetState(Edead);
	}
}