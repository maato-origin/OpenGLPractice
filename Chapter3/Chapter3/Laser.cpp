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
	//スプライトコンポーネントを作成
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("../Assets/Laser.png"));

	//MoveComponentを作成し、前進速度を設定
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.0f);

	//CircleComponentを作成(コリジョン用)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime)
{
	//時間切れでレーザーを消す
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.0f)
	{
		SetState(Edead);
	}
	else
	{
		//小惑星と交差するか？
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				//レーザーが小惑星と交差するなら
				//レーザーも小惑星の消えてなくなる
				SetState(Edead);
				ast->SetState(Edead);
				break;
			}
		}
	}
}