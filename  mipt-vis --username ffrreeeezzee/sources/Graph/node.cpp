/**
 * File: node.cpp - Node class implementation, part of Graph library, 
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"

/** 
 * Destructor. Corrects list of nodes in corresponding graph and deletes adjacent edges
 */
Node::~Node()
{
    Edge *edge;
    
    /** Delete incidient edges */
    for ( edge = GetFirstSucc(); !EndOfSuccs();)
    {
        Edge* next = GetNextSucc();
        edge->DetachFromNode( GRAPH_DIR_DOWN);// Edge is detached from succ node
        delete edge;
        edge = next;
    }
    for ( edge = GetFirstPred(); !EndOfPreds();)
    {
        Edge* next = GetNextPred();
        edge->DetachFromNode( GRAPH_DIR_UP);// Edge is detached from pred node
        delete edge;
        edge = next;
    }
    /** Delete myself from graph */
    graph->DeleteNode( &my_it);
}

/**
 * Add an edge to this node in specified direction
 */
void
Node::AddEdgeInDir( Edge *edge, GraphDir dir)
{
    GraphAssert( IsNotNullP( edge));
    EdgeListItem *it = edge->GetNodeIt( RevDir( dir));
    it->Attach( edges[ dir]);
    edges[ dir] = it;
}

/**
 * Delete edge pointed by iterator in specidied direction
 */
void
Node::DeleteEdgeInDir( GraphDir dir, EdgeListItem* it)
{
    GraphAssert( IsNotNullP( it));
    if( edges[ dir] == it)
    {
        edges[ dir] = it->GetNext();
    }
    if( e_it[ dir] == it)
    {
        e_it[ dir] = it->GetNext();
    }
    it->Detach();
}

/**
 * Print node in Dot format to stdout
 */
void
Node::DebugPrint()
{
    out("%u;", GetId());
}