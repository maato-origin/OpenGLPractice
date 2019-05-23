#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>

struct GraphNode
{
	//�e�m�[�h�͗אڃm�[�h�ւ̃|�C���^������
	std::vector<GraphNode*> mAdjacent;
};

struct Graph
{
	//�O���t�̓m�[�h���܂�
	std::vector<GraphNode*> mNodes;
};

struct WeightedEdge
{
	//�G�b�W�Ɍq�����Ă���m�[�h
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;
	//�G�b�W�̏d��
	float mWeight;
};

struct WeightedGraphNode
{
	//���̃m�[�h����o�Ă����G�b�W���i�[
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

	//�J�����g�m�[�h���n�_�m�[�h�ɐݒ�A���N���[�Y�h�Z�b�g�ɒǉ�
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		//�אڃm�[�h���I�[�v���Z�b�g�ɒǉ�����
		for (const WeightedEdge* edge : current->mEdges)
		{
			const WeightedGraphNode* neighbor = edge->mTo;
			//���̃m�[�h�̃X�N���b�`�f�[�^���擾
			AStarScratch& data = outMap[neighbor];
			//�N���[�Y�h�Z�b�g�ɂȂ��m�[�h�������`�F�b�N
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					//�I�[�v���Z�b�g�ɂȂ���ΐe�̓J�����g�ɈႢ�Ȃ�
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					//���ۂ̃R�X�g�́A�e�̃R�X�g�{�G�b�W��H��R�X�g
					data.mActualFromStart = outMap[current].mActualFromStart + edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					//�J�����g��e�ɂ������̌o�H�R�X�g���v�Z
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart)
					{
						//���̃R�[�h�̐e���J�����g�ɂ���
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		//�I�[�v���Z�b�g����Ȃ�Ε]�����ׂ��m�[�h�͎c���Ă��Ȃ�
		if (openSet.empty())
		{
			break;
		}

		//�ł��R�X�g�̒Ⴂ�m�[�h���I�[�v���Z�b�g����T��
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		//������J�����g�m�[�h�ɂ��āA�I�[�v���Z�b�g����N���[�Y�h�Z�b�g�Ɉڂ�
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	//�o�H��������ꂽ��
	return (current == goal) ? true : false;

}

bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	//�J�����g�m�[�h���n�_�m�[�h�ɐݒ�A���N���[�Y�h�Z�b�g�ɒǉ�
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		//�אڃm�[�h���I�[�v���Z�b�g�ɒǉ�����
		for (const WeightedEdge* edge : current->mEdges)
		{
			//���̃m�[�h�̃X�N���b�`�f�[�^���擾
			GBFSScratch& data = outMap[edge->mTo];

			//�N���[�Y�h�Z�b�g�ɂȂ��Ƃ��Ɍ���ǉ�
			if (!data.mInClosedSet)
			{
				//�אڃm�[�h�̐e�G�b�W��ݒ肷��
				data.mParentEdge = edge;
				if (!data.mInOpenSet)
				{
					//�q���[���X�e�B�b�N���v�Z���ăI�[�v���Z�b�g�ɒǉ�����
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		//�I�[�v���Z�b�g����Ȃ�Ε]�����ׂ��m�[�h�͎c���Ă��Ȃ�
		if (openSet.empty())
		{
			break;
		}

		//�ł��R�X�g�̒Ⴂ�m�[�h���I�[�v���Z�b�g����T��
		auto iter=std::min_element(openSet.begin(),openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		//������J�����g�m�[�h�ɂ��āA�I�[�v���Z�b�g����N���[�Y�h�Z�b�g�Ɉڂ�
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	//�o�H��������ꂽ��
	return (current == goal) ? true : false;
}

using NodeToParentMap =
std::unordered_map<const GraphNode*, const GraphNode*>;

bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap)
{
	//�o�H����������
	bool pathFound = false;
	//��������m�[�h
	std::queue<const GraphNode*> q;
	//�ŏ��̃m�[�h���L���[�ɓ����
	q.emplace(start);

	while (!q.empty())
	{
		//�m�[�h��1�L���[����o��
		const GraphNode* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		//�܂��L���[�ɓ����Ă��Ȃ��אڃm�[�h���G���L���[����
		for (const GraphNode* node : current->mAdjacent)
		{
			//�����e��nullptr�Ȃ�܂��L���[�ɒǉ�����Ă��Ȃ�
			//(�������J�n�m�[�h�͗�O)
			const GraphNode* parent = outMap[node];
			if (parent == nullptr&&node != start)
			{
				//���̃m�[�h�̃G���L���[�Ɛe�̐ݒ������
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
	//�q�m�[�h�̃��X�g
	std::vector<GTNode*> mChildren;
	//���̃m�[�h�̃Q�[���̏��
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
	//�ǂꂩ�̍s����v���Ă��邩
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

	//�ǂꂩ�̗񂪈�v���Ă��邩
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

	//�Ίp���̔���
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

	//��������
	return 0.0f;
}

float MinPlayer(const GTNode* node);

float MaxPlayer(const GTNode* node)
{
	//���̃m�[�h�����[�t�Ȃ�A�X�R�A��Ԃ�
	if (node->mChildren.empty())
	{
		return GetScore(node->mState);
	}

	//�ő�l�̃T�u�c���[��T��
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren)
	{
		maxValue = std::max(maxValue, MinPlayer(child));
	}
	return maxValue;
}

float MinPlayer(const GTNode* node)
{
	//���̃m�[�h�����[�t�Ȃ�A�X�R�A��Ԃ�
	if (node->mChildren.empty())
	{
		return GetScore(node->mState);
	}

	//�ŏ��l�̃T�u�c���[��T��
	float minValue = std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren)
	{
		minValue = std::min(minValue, MaxPlayer(child));
	}
	return minValue;
}

const GTNode* MinimaxDecide(const GTNode* root)
{
	//�ő�̒l�����T�u�c���[��T���āA���̑I����Ԃ�
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
	//���ꂪ���[�t�Ȃ�X�R�A��Ԃ�
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
			return maxValue;	//beta��؂���
		}
		alpha = std::max(maxValue, alpha);
	}
	return maxValue;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta)
{
	//���ꂪ���[�t�Ȃ�X�R�A��Ԃ�
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
			return minValue;	//alpha��؂���
		}
		beta = std::min(minValue, beta);
	}
	return minValue;
}

const GTNode* AlphaBetaDecide(const GTNode* root)
{
	const GTNode* choice = nullptr;
	//alpha�̓}�C�i�X������Abeta�͖����傩��J�n
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