#include "ALGraph.h"

#define INFINITY static_cast<unsigned>(-1);

struct compare
{
	bool operator()(AdjacencyInfo Left, AdjacencyInfo Right)
	{
		return Left.weight > Right.weight;
	};
};

ALGraph::ALGraph(unsigned size)
{
	AList = ALIST(size);
	DInfo = std::vector<DijkstraInfo>(size);

	for(unsigned i = 0; i < DInfo.size(); i++)
		DInfo[i].cost = INFINITY;

	eval = std::vector<bool>(size, false);
	size_ = size;
}

template <typename T>
static void swap(T& left, T& right)
{
	T temp = left;
	left = right;
	right = temp;
}

ALGraph::~ALGraph()
{

}

bool operator<(const AdjacencyInfo& Left, const AdjacencyInfo& Right)
{
	if(Left.weight == Right.weight)
		return Left.id < Right.id;
	return Left.weight < Right.weight;
}

void ALGraph::AddDEdge(unsigned source, unsigned destination, unsigned weight)
{

  AList[source - 1].push_back({destination, weight});

  for(long unsigned i = AList[source -1].size() - 1; i > 0; i--)
  {
  	if(AList[source - 1][i - 1] < AList[source - 1][i])
  		break;
  	swap(AList[source - 1][i], AList[source - 1][i - 1]);
  }
}

void ALGraph::AddUEdge(unsigned node1, unsigned node2, unsigned weight)
{
  AddDEdge(node1, node2, weight);
  AddDEdge(node2, node1, weight);
}

ALIST ALGraph::GetAList() const
{
	return AList;
}

std::vector<DijkstraInfo> ALGraph::Dijkstra(unsigned start_node) const
{
	std::priority_queue<AdjacencyInfo, std::vector<AdjacencyInfo>, compare> Queue;
	AdjacencyInfo node;

	DInfo[start_node - 1].cost = 0;
	DInfo[start_node - 1].path.push_back(start_node);
	eval[start_node - 1] = true;

	for(long unsigned i = 0; i < AList[start_node - 1].size(); i++) 
	{
		node = AList[start_node - 1][i];

		DInfo[node.id - 1] = DInfo[start_node - 1];
		DInfo[node.id - 1].cost += node.weight;
		DInfo[node.id - 1].path.push_back(node.id);
		Queue.push(node);
	}

	while(!Queue.empty())
	{
		node = Queue.top();
		Queue.pop();

		if(eval[node.id - 1] == true)
			continue;
		else
		{
			eval[node.id - 1] = true;
			unsigned current_id = node.id;

			for(long unsigned i = 0; i < AList[current_id - 1].size(); i++) 
			{
				node = AList[current_id - 1][i];

				if(node.weight + DInfo[current_id - 1].cost < DInfo[node.id - 1].cost)
				{
					DInfo[node.id - 1] = DInfo[current_id - 1];
					DInfo[node.id - 1].cost += node.weight;
					DInfo[node.id - 1].path.push_back(node.id);
					node.weight = DInfo[node.id - 1].cost;
					Queue.push(node);
				}
			}
		}
	}
	return DInfo;
}

/////////////////////////////////////////////////////////////////////////
// Extra Credit  // // // // // // // // // // // // // // // // // // //
/////////////////////////////////////////////////////////////////////////
bool ALGraph::ImplementedSearches()
{
	return false;
}

std::vector<unsigned> ALGraph::SearchFrom(unsigned , TRAVERSAL_METHOD ) const
{
	std::vector<unsigned> v;
	return v;
}

//////////////////////////////////////////////////////////////////////////
// // // // // // // // // // // // // // // // // // // // // // // // //
//////////////////////////////////////////////////////////////////////////
