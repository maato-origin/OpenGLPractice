#pragma once
#include "Actor.h"
#include <vector>

class Grid : public Actor
{
public:
	Grid(class Game* game);

	//特定のx/yクリック時のマウス操作
	void ProcessClick(int x, int y);

	//パス検索にA*を使用
	bool FindPath(class Tile* start, class Tile* goal);

	//タワー建築の試み
	void BuildTower();

	//start/endタイルの取得
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	void UpdateActor(float deltaTime) override;

private:
	//特定のタイルを選択
	void SelectTile(size_t row, size_t col);

	//パスのタイルテクスチャを更新
	void UpdatePathTiles(class Tile* start);

	//今選択されたタイル
	class Tile* mSelectedTile;

	//グリッド内の2D配列のタイル
	std::vector<std::vector<class Tile*>> mTiles;

	//次の敵出現までの時間
	float mNextEnemy;

	//グリッド内の行列
	const size_t NumRows = 7;
	const size_t NumCols = 16;
	//左上隅のy座標開始位置
	const float StartY = 192.0f;
	//各タイルの幅と高さ
	const float TileSize = 64.0f;
	//敵の出現間隔
	const float EnemyTime = 1.5f;
};