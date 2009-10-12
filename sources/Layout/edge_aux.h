#ifndef EDGE_AUX_H
#define EDGE_AUX_H

/**
 * Auxiliary edge representation class. 
 */

class EdgeAux : public Edge
{
    friend class GraphAux;
    friend class NodeAux;

	enum
	{
		tree,
		forward,
		back,
		mesh //the same as: A->A
	} type;

protected:
    EdgeAux( GraphAux *graph_p, int _id, NodeAux* _pred, NodeAux* _succ);
	virtual void debugPrint();
public:
    inline EdgeAux* nextEdge()
	{
		return static_cast< EdgeAux*> ( Edge::nextEdge());
	}
	
	inline NodeAux* getNode( GraphDir dir);
	inline NodeAux* succ();
	inline NodeAux* pred();
};

#endif
