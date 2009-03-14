/**
 * File: graph.cpp - Graph class implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"

/**
 * Constructor.
 */
Graph::Graph()
{
    node_next_id = 0;
    edge_next_id = 0;
    node_num = 0;
    edge_num = 0;
	max_node_id = 0;
    nodes = NULL;
    edges = NULL;
    n_it = NULL;
    e_it = NULL;
}

/**
 * Creation node in graph
 */
Node * 
Graph::NewNode()
{
    /**
     * Check that we have available node id 
     */
    GraphAssert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Node *node_p = new Node( this, node_next_id++);
    NodeListItem* it = node_p->GetGraphIt();
    it->Attach( nodes);
    nodes = it;
    node_num++;
    return node_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
Edge * 
Graph::NewEdge( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    GraphAssert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = new Edge( this, edge_next_id++, pred, succ);
    EdgeListItem* it = edge_p->GetGraphIt();
    it->Attach( edges);
    edges = it;
    edge_num++;
    return edge_p;
}

/**
 * Print graph to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
void 
Graph::DebugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = GetFirstNode(); !EndOfNodes(); n = GetNextNode())
    {
        n->DebugPrint();
    }
    /** Print edges */
    for (  e = GetFirstEdge(); !EndOfEdges(); e = GetNextEdge())
    {
        e->DebugPrint();
    }
    out( "}");
}

/**
  * Recursive implementation of depth-first search visit routine
  */
static inline NodeListItem* DfsVisitRec( Node* node, NodeListItem* item, Marker m)
{
    NodeListItem* new_item = new NodeListItem( item, LIST_DIR_RDEFAULT, node);
    Edge *e;
    
    /** Mark node to prevent search from visiting it again */
    node->Mark( m);
    node->DebugPrint();// Print node ID. TODO: Remove this when numeration implementation finished.

    /** Visit Succs */
    for ( e = node->GetFirstSucc(); !node->EndOfSuccs(); e = node->GetNextSucc())
    {
        Node* succ = e->GetSucc();
        if ( !succ->IsMarked( m))
        {
            new_item = DfsVisitRec( succ, new_item, m);
        }
    }
    return new_item;
}

/**
 * Implementation of depth-first search. Starts from nodes without predecessors.
 */
NodeListItem* Graph::DFS()
{
    Node *n;
    GraphNum num = 0;
    NodeListItem *item = NULL;
    Marker m = NewMarker();

    /** Visit nodes */
    for (  n = GetFirstNode(); !EndOfNodes(); n = GetNextNode())
    {
        if( IsNullP( n->GetFirstPred()))
        {
            GraphAssert( !n->IsMarked( m));
            item = DfsVisitRec( n, item, m);
        }
    }   
    
    FreeMarker( m);

    return item;
}

