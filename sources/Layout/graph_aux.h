/**
 * File: graph_aux.h - Auxiliary graph class definition/implementation, part of
 * Layout library in MiptVis tool.
 * Copyright (C) 2009  MIPTVIS
 */

#ifndef GRAPH_AUX_H
#define GRAPH_AUX_H

/**
 * Implementation of auxiliary graph class
 */
class GraphAux: public Graph
{
private:
	NodeAux* getFirstNode();
	NodeAux* makeAcyclic();//needs for Rank, changes types of edges
	void makeAcyclicImp (NodeAux* from, Marker passed, Marker ret);
	void rankImp (NodeAux* from, int cur_rank);
public://!!! Only for tests, it must be closed
	bool rank();
public:
    /** Constructor */
    GraphAux(): Graph()
    {};

    /** Allocation of memory for Edge */
    EdgeAux * createEdge( Node * pred, Node * succ);
				//???type NodeAux not lead to Node, and call "creatEdge (Node *, Node*)" not work
    /** Allocation of memory for Node */
    NodeAux * createNode();

    /**
    * Creation node in graph
    */
    NodeAux * newNode()
    {
        return static_cast< NodeAux*>( Graph::newNode());
    }

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    EdgeAux * newEdge( NodeAux * pred, NodeAux * succ)
    {
        return static_cast< EdgeAux*>( Graph::newEdge( static_cast< Node*>( pred), static_cast< Node*>( succ)));
    };
    
};

#endif