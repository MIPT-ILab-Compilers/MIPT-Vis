/**
 * File: Layout/layout_iface.h - Interface of layout library,
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef LAYOUT_IFACE_H
#define LAYOUT_IFACE_H

#include "layout_predecls.h"
#include "graph_aux.h"
#include "node_aux.h"
#include "edge_aux.h"

NodeAux::NodeAux( GraphAux *graph_p, int _id):
	Node( static_cast<Graph *>( graph_p), _id)
	{};
EdgeAux::EdgeAux( GraphAux *graph_p, int _id, NodeAux* _pred, NodeAux* _succ):
	Edge( static_cast< Graph*>( graph_p), _id,  static_cast< Node*>( _pred),  static_cast< Node*>( _succ))
	{};

void *GraphAux::createEdge( NodeAux * pred, NodeAux * succ)
{
	return new EdgeAux( this, incEdgeId(), pred, succ);
};
  
void *GraphAux::createNode()
{
	return new NodeAux( this, incNodeId());
}
	

#endif