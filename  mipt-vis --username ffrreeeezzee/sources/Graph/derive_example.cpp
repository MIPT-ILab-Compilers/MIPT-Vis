/**
 * File: derive_example.cpp Example of deriving classes from graph. Here we have DerivedGraph - DGraph.
 * Its redefined only partly : its node have additional attributes, but edges don not.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_iface.h"

class DGraph; // Predeclaration
/**
 * Class Derived from Node - DNode. 
 * Illustrates how you can make nodes with additional data/functionality
 */
class DNode: public Node
{
    int val;
public:
    DNode( DGraph *graph, int id);
};
/**
 * Minimal code needed to make custom graph
 */
class DGraph: public Graph
{
public:
    
    void *CreateNode()
    {
        return new DNode( this, IncNodeId());
    }
    DNode *NewNode()
    {
        return static_cast< DNode *>( Graph::NewNode());
    }
    DNode * GetFirstNode()
    {
        return static_cast< DNode *>( Graph::GetFirstNode());
    }
    DNode * GetNextNode()
    {
        return static_cast< DNode *>( Graph::GetNextNode());
    }
};

DNode::DNode( DGraph *graph, int _id):
    Node( static_cast<Graph *>( graph), _id)
{
    val = 0;
}
