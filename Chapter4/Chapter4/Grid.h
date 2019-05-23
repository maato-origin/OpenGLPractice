#pragma once
#include "Actor.h"
#include <vector>

class Grid : public Actor
{
public:
	Grid(class Game* game);

	//�����x/y�N���b�N���̃}�E�X����
	void ProcessClick(int x, int y);

	//�p�X������A*���g�p
	bool FindPath(class Tile* start, class Tile* goal);

	//�^���[���z�̎���
	void BuildTower();

	//start/end�^�C���̎擾
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	void UpdateActor(float deltaTime) override;

private:
	//����̃^�C����I��
	void SelectTile(size_t row, size_t col);

	//�p�X�̃^�C���e�N�X�`�����X�V
	void UpdatePathTiles(class Tile* start);

	//���I�����ꂽ�^�C��
	class Tile* mSelectedTile;

	//�O���b�h����2D�z��̃^�C��
	std::vector<std::vector<class Tile*>> mTiles;

	//���̓G�o���܂ł̎���
	float mNextEnemy;

	//�O���b�h���̍s��
	const size_t NumRows = 7;
	const size_t NumCols = 16;
	//�������y���W�J�n�ʒu
	const float StartY = 192.0f;
	//�e�^�C���̕��ƍ���
	const float TileSize = 64.0f;
	//�G�̏o���Ԋu
	const float EnemyTime = 1.5f;
};