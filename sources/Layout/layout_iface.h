/**
 * File: Layout/layout_iface.h - Interface of layout library,
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef LAYOUT_IFACE_H
#define LAYOUT_IFACE_H

#include "layout_predecls.h"
#include "edge_aux.h"
#include "node_aux.h"
#include "graph_aux.h"

/**
 * Get node's corresponding auxiliary graph
 */
inline GraphAux * NodeAux::getGraph() const
{
	return static_cast< GraphAux *>( Node::getGraph());
}

inline NodeAux::NodeAux( GraphAux *graph_p, int _id):
	Node( static_cast<Graph *>( graph_p), _id)
	{};
inline EdgeAux::EdgeAux( GraphAux *graph_p, int _id, NodeAux* _pred, NodeAux* _succ):
	Edge( static_cast< Graph*>( graph_p), _id,  static_cast< Node*>( _pred),  static_cast< Node*>( _succ))
	{};

inline EdgeAux *GraphAux::createEdge( NodeAux * pred, NodeAux * succ)
{
	return new EdgeAux( this, incEdgeId(), pred, succ);
};
  
inline NodeAux *GraphAux::createNode()
{
	return new NodeAux( this, incNodeId());
}
	

#endif
