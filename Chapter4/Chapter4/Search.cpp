#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>

struct GraphNode
{
	//各ノードは隣接ノードへのポインタを持つ
	std::vector<GraphNode*> mAdjacent;
};

struct Graph
{
	//グラフはノードを含む
	std::vector<GraphNode*> mNodes;
};

struct WeightedEdge
{
	//エッジに繋がっているノード
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;
	//エッジの重み
	float mWeight;
};

struct WeightedGraphNode
{
	//このノードから出ていくエッジを格納
	std::vector<WeightedEdge*> mEdges;
};

struct WeightedGraph
{
	std::vector<WeightedGraphNode*> mNodes;
};

struct GBFSScratch
{
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.0f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

using GBFSMap =
std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

struct AStarScratch
{
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.0f;
	float mActualFromStart = 0.0f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

using AStarMap =
std::unordered_map<const WeightedGraphNode*, AStarScratch>;

float ComputeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b)
{
	return 0.0f;
}

bool AStar(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, AStarMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	//カレントノードを始点ノードに設定、かつクローズドセットに追加
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		//隣接ノードをオープンセットに追加する
		for (const WeightedEdge* edge : current->mEdges)
		{
			const WeightedGraphNode* neighbor = edge->mTo;
			//このノードのスクラッチデータを取得
			AStarScratch& data = outMap[neighbor];
			//クローズドセットにないノードだけをチェック
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					//オープンセットになければ親はカレントに違いない
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					//実際のコストは、親のコスト＋エッジを辿るコスト
					data.mActualFromStart = outMap[current].mActualFromStart + edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					//カレントを親にした時の経路コストを計算
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart)
					{
						//このコードの親をカレントにする
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		//オープンセットが空ならば評価すべきノードは残っていない
		if (openSet.empty())
		{
			break;
		}

		//最もコストの低いノードをオープンセットから探す
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		//それをカレントノードにして、オープンセットからクローズドセットに移す
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	//経路を見つけられたか
	return (current == goal) ? true : false;

}

bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	//カレントノードを始点ノードに設定、かつクローズドセットに追加
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		//隣接ノードをオープンセットに追加する
		for (const WeightedEdge* edge : current->mEdges)
		{
			//このノードのスクラッチデータを取得
			GBFSScratch& data = outMap[edge->mTo];

			//クローズドセットにないときに限り追加
			if (!data.mInClosedSet)
			{
				//隣接ノードの親エッジを設定する
				data.mParentEdge = edge;
				if (!data.mInOpenSet)
				{
					//ヒューリスティックを計算してオープンセットに追加する
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		//オープンセットが空ならば評価すべきノードは残っていない
		if (openSet.empty())
		{
			break;
		}

		//最もコストの低いノードをオープンセットから探す
		auto iter=std::min_element(openSet.begin(),openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		//それをカレントノードにして、オープンセットからクローズドセットに移す
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	//経路を見つけられたか
	return (current == goal) ? true : false;
}

using NodeToParentMap =
std::unordered_map<const GraphNode*, const GraphNode*>;

bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap)
{
	//経路を見つけたか
	bool pathFound = false;
	//検討するノード
	std::queue<const GraphNode*> q;
	//最初のノードをキューに入れる
	q.emplace(start);

	while (!q.empty())
	{
		//ノードを1つキューから出す
		const GraphNode* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		//まだキューに入っていない隣接ノードをエンキューする
		for (const GraphNode* node : current->mAdjacent)
		{
			//もし親がnullptrならまだキューに追加されていない
			//(ただし開始ノードは例外)
			const GraphNode* parent = outMap[node];
			if (parent == nullptr&&node != start)
			{
				//このノードのエンキューと親の設定をする
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;
}

void testBFS()
{
	Graph g;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			GraphNode* node = new GraphNode;
			g.mNodes.emplace_back(node);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			GraphNode* node = g.mNodes[i * 5 + j];
			if (i > 0)
			{
				node->mAdjacent.emplace_back(g.mNodes[(i - 1) * 5 + j]);
			}
			if (i < 4)
			{
				node->mAdjacent.emplace_back(g.mNodes[(i + 1) * 5 + j]);
			}
			if (j > 0)
			{
				node->mAdjacent.emplace_back(g.mNodes[i * 5 + j - 1]);
			}
			if (j < 4)
			{
				node->mAdjacent.emplace_back(g.mNodes[i * 5 + j + 1]);
			}
		}
	}

	NodeToParentMap map;
	bool found = BFS(g, g.mNodes[0], g.mNodes[9], map);
	std::cout << found << '\n';
}

struct GameState
{
	enum SquareState { Empty, X, O };
	SquareState mBoard[3][3];
};

struct GTNode
{
	//子ノードのリスト
	std::vector<GTNode*> mChildren;
	//このノードのゲームの状態
	GameState mState;
};

void GenStates(GTNode* root, bool xPlayer)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (root->mState.mBoard[i][j] == GameState::Empty)
			{
				GTNode* node = new GTNode;
				root->mChildren.emplace_back(node);
				node->mState = root->mState;
				node->mState.mBoard[i][j] = xPlayer ? GameState::X : GameState::O;
				GenStates(node, !xPlayer);
			}
		}
	}
}

float GetScore(const GameState& state)
{
	//どれかの行が一致しているか
	for (int i = 0; i < 3; i++)
	{
		bool same = true;
		GameState::SquareState v = state.mBoard[i][0];
		for (int j = 1; j < 3; j++)
		{
			if (state.mBoard[i][j] != v)
			{
				same = false;
			}
		}

		if (same)
		{
			if (v == GameState::X)
			{
				return 1.0f;
			}
			else
			{
				return -1.0f;
			}
		}
	}

	//どれかの列が一致しているか
	for (int j = 0; j < 3; j++)
	{
		bool same = true;
		GameState::SquareState v = state.mBoard[0][j];
		for (int i = 1; i < 3; j++)
		{
			if (state.mBoard[i][j] != v)
			{
				same = false;
			}
		}

		if (same)
		{
			if (v == GameState::X)
			{
				return 1.0f;
			}
			else
			{
				return -1.0f;
			}
		}
	}

	//対角線の判定
	if(((state.mBoard[0][0]==state.mBoard[1][1]) &&
		(state.mBoard[1][1] == state.mBoard[2][2])) ||
		((state.mBoard[2][0] == state.mBoard[1][1]) &&
		(state.mBoard[1][1] == state.mBoard[0][2])))
	{
		if (state.mBoard[1][1] == GameState::X)
		{
			return 1.0f;
		}
		else
		{
			return -1.0f;
		}
	}

	//引き分け
	return 0.0f;
}

float MinPlayer(const GTNode* node);

float MaxPlayer(const GTNode* node)
{
	//このノードがリーフなら、スコアを返す
	if (node->mChildren.empty())
	{
		return GetScore(node->mState);
	}

	//最大値のサブツリーを探す
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren)
	{
		maxValue = std::max(maxValue, MinPlayer(child));
	}
	return maxValue;
}

float MinPlayer(const GTNode* node)
{
	//このノードがリーフなら、スコアを返す
	if (node->mChildren.empty())
	{
		return GetScore(node->mState);
	}

	//最小値のサブツリーを探す
	float minValue = std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren)
	{
		minValue = std::min(minValue, MaxPlayer(child));
	}
	return minValue;
}

const GTNode* MinimaxDecide(const GTNode* root)
{
	//最大の値を持つサブツリーを探して、その選択を返す
	const GTNode* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : root->mChildren)
	{
		float v = MinPlayer(child);
		if (v > maxValue)
		{
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta);

float AlphaBetaMax(const GTNode* node, float alpha, float beta)
{
	//これがリーフならスコアを返す
	if (node->mChildren.empty())
	{
		return GetScore(node->mState);
	}

	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren)
	{
		maxValue = std::max(maxValue, AlphaBetaMin(child, alpha, beta));
		if (maxValue >= beta)
		{
			return maxValue;	//betaを切り取る
		}
		alpha = std::max(maxValue, alpha);
	}
	return maxValue;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta)
{
	//これがリーフならスコアを返す
	if (node->mChildren.empty())
	{
		return GetScore(node->mState);
	}

	float minValue = std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren)
	{
		minValue = std::min(minValue, AlphaBetaMax(child, alpha, beta));
		if (minValue <= alpha)
		{
			return minValue;	//alphaを切り取る
		}
		beta = std::min(minValue, beta);
	}
	return minValue;
}

const GTNode* AlphaBetaDecide(const GTNode* root)
{
	const GTNode* choice = nullptr;
	//alphaはマイナス無限大、betaは無限大から開始
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	for (const GTNode* child : root->mChildren)
	{
		float v = AlphaBetaMin(child, maxValue, beta);
		if (v > maxValue)
		{
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}