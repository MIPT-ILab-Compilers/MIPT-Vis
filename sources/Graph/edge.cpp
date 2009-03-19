/**
 * File: edge.cpp - Edge class implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"

/**
 * Edge destructor.
 * delete edge from graph's list of edges
 */

Edge::~Edge()
{
    graph->deleteEdge( &graph_it);
}

/**
 * Print edge in DOT format to stdout
 */

void
Edge::debugPrint()
{
    /**
     * Check that edge is printable
     * TODO: Implements graph states and in 'in process' state print node as '?'
     *       Examples of such prints: 4->? ?->3 ?->?
     */
    graphassert( isNotNullP( pred()));
    graphassert( isNotNullP( succ()));

    out("%u->%u;", pred()->id(), succ()->id());
}

/**
 * Low level correction of node's edge list in corresponding direction
 */

void
Edge::detachFromNode( GraphDir dir)
{
    Node *n = getNode( dir);
    n->deleteEdgeInDir( revDir( dir), &n_it[ dir]);
}