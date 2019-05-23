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
	//ランダムな位置/向きで初期化
	Vector2 randPos = Random::GetVector(Vector2(-512.0f,-384.0f),
		Vector2(512.0f, 384.0f));
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	//スプライトコンポーネントを作成し、テクスチャを設定
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//運動コンポーネントを作成し、前進速度を設定
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);

	//CircleComponentを作成(コリジョン用)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	//mAsteroidsをgameに加える
	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}