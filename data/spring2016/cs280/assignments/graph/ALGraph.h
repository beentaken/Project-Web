//---------------------------------------------------------------------------
#ifndef ALGRAPH_H
#define ALGRAPH_H
//---------------------------------------------------------------------------
#include <vector>
#include <queue>
#include <functional> 

struct DijkstraInfo
{
  unsigned cost;
  std::vector<unsigned> path;
};

struct AdjacencyInfo
{
  unsigned id;
  unsigned weight;
};

typedef std::vector< std::vector< AdjacencyInfo > > ALIST;

class ALGraph
{
  public:
    enum TRAVERSAL_METHOD {DEPTH_FIRST, BREADTH_FIRST};

    ALGraph(unsigned size);
    ~ALGraph();
    void AddDEdge(unsigned source, unsigned destination, unsigned weight);
    void AddUEdge(unsigned node1, unsigned node2, unsigned weight);

    std::vector<unsigned> SearchFrom(unsigned start_node, TRAVERSAL_METHOD method) const;
    std::vector<DijkstraInfo> Dijkstra(unsigned start_node) const;
    ALIST GetAList() const;
    static bool ImplementedSearches();
        
  private:
  	bool evaluated() const;

    ALIST AList;
    mutable std::vector<DijkstraInfo> DInfo;
    mutable std::vector<bool> eval;
    int size_;
};
/*
	Choose a node to be the source or starting point.
	Initialize source to 0 cost and mark as evaluated.
	Initialize all nodes to infinite cost from the source.

	For each node, y, adjacent to source
	  1. Relax the node. That is, set y's cost to the cost of all edges from source to y.
	  2. Place y into a priority queue based on its total cost. (Lower is better)
	  3. Add source node as predecessor of y.
	End For
	
	While there are nodes in the graph that haven't been evaluated
	  Remove a node, x, from the PQ (lowest total cost)
	  If the node has already been evaluated
	    Discard the node
	    Go to top of while 
	  Else
	    Mark x as evaluated.
	    For each neighbor, y, of x
	      Relax y
	      If new cost to reach y is less
	        Update list of nodes (path) to y from source.
	        Place y in the PQ.
	      End If
	    End For
	  End If
	End While
*/

#endif
