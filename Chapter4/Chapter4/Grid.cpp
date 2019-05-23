#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>

Grid::Grid(class Game* game)
	:Actor(game)
	, mSelectedTile(nullptr)
{
	//7�s16��
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}

	//�^�C���̍쐬
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(Vector2(TileSize / 2.0f + j * TileSize, StartY + i * TileSize));
		}
	}

	//start/end�^�C����ݒ�
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	//�אڃ��X�g�̐ݒ�
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
			}
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
			}
		}
	}

	//�p�X�̔���(�t)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	mNextEnemy = EnemyTime;
}

void Grid::SelectTile(size_t row, size_t col)
{
	//�������I�����m�F����
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase)
	{
		//�O�̈��I�����Ȃ�
		if (mSelectedTile)
		{
			mSelectedTile->ToggleSelect();
		}
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}

void Grid::ProcessClick(int x, int y)
{
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

//A*�o�H�T�������s
bool Grid::FindPath(Tile* start, Tile* goal)
{
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j]->g = 0.0f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;

	//�J�����g�m�[�h��start��ݒ肵�A������N���[�Y�h�Z�b�g�ɉ�����
	Tile* current = start;
	current->mInClosedSet = true;

	do
	{
		//�אڃm�[�h���I�[�v���Z�b�g�ɉ�����
		for (Tile* neighbor : current->mAdjacent)
		{
			if (neighbor->mBlocked)
			{
				continue;
			}

			//�N���[�Y�h�Z�b�g�ɂȂ��m�[�h�������`�F�b�N
			if (!neighbor->mInClosedSet)
			{
				if (!neighbor->mInOpenSet)
				{
					//�I�[�v���Z�b�g�ɂȂ���ΐe��ݒ�
					neighbor->mParent = current;
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					//g(x)�͐e��g�Ɉړ��R�X�g��������
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->mInOpenSet = true;
				}
				else
				{
					//�J�����g��e�ɂ����Ƃ���g(x)�R�X�g
					float newG = current->g + TileSize;
					if (newG < neighbor->g)
					{
						//���̃m�[�h��K��
						neighbor->mParent = current;
						neighbor->g = newG;
						//g(x)�̕ω��ɂ��f(x)���ω�����
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		//�I�[�v���Z�b�g����Ȃ�Ε]�����ׂ��m�[�h�͎c���Ă��Ȃ�
		if (openSet.empty())
		{
			break;
		}

		//�I�[�v���Z�b�g�̒�����ŏ��R�X�g�̃m�[�h��T��
		auto iter=std::min_element(openSet.begin(),openSet.end(),
			[](Tile* a, Tile* b) {
			return a->f < b->EDefault;
		});

		//������J�����g�m�[�h�ɂ��āA�I�[�v���Z�b�g����N���[�Y�h�Z�b�g�Ɉڂ�
		current = *iter;
		openSet.erase(iter);
		current->mInOpenSet = false;
		current->mInClosedSet = true;
	} while (current != goal);

	//�o�H��������ꂽ��
	return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(class Tile* start)
{
	//�S�Ẵ^�C�������ɖ߂�(start/end������)
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15))
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}

	Tile* t = start->mParent;
	while (t != GetEndTile())
	{
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}

void Grid::BuildTower()
{
	if (mSelectedTile && !mSelectedTile->mBlocked)
	{
		mSelectedTile->mBlocked = true;
		if (FindPath(GetEndTile(), GetStartTile()))
		{
			Tower* t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else
		{
			//���̃^���[���o�H���Ȃ����Ȃ�΁A���݂������Ȃ�
			mSelectedTile->mBlocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

Tile* Grid::GetStartTile()
{
	return mTiles[3][0];
}

Tile* Grid::GetEndTile()
{
	return mTiles[3][15];
}

void Grid::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	//�V�����G�𐶐����鎞�Ԃ�
	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.0f)
	{
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}