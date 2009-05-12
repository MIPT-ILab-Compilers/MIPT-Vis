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
public:
    /** Constructor */
    GraphAux(): Graph()
    {};

    /** Allocation of memory for Edge */
    EdgeAux * createEdge( NodeAux * pred, NodeAux * succ);
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