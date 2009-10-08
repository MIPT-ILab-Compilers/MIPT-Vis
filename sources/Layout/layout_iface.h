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

const int Big_way_len_value = 10000000;

/**
 * Get node's corresponding auxiliary graph
 */
inline GraphAux * NodeAux::getGraph() const
{
    return static_cast< GraphAux *>( Node::getGraph());
}

inline NodeAux::NodeAux( GraphAux *graph_p, int _id):
    Node( static_cast<Graph *>( graph_p), _id)
{}

inline EdgeAux::EdgeAux( GraphAux *graph_p, int _id, NodeAux* _pred, NodeAux* _succ):
    Edge( static_cast< Graph*>( graph_p), _id,  static_cast< Node*>( _pred),  static_cast< Node*>( _succ)),
	type (tree)
{}

inline EdgeAux *GraphAux::createEdge( Node * pred, Node * succ)
{
    return new EdgeAux( this, incEdgeId(), static_cast<NodeAux*> (pred), static_cast<NodeAux*> (succ));
}
  
inline NodeAux *GraphAux::createNode()
{
    return new NodeAux( this, incNodeId());
}

#define CUT_AUX_DEFINITION(type)			\
inline type* cutAux (type##Aux *what)		\
{											\
	return static_cast <type*> (what);		\
}
#define ADD_AUX_DEFINITION(type)			\
inline type##Aux* addAux (type* what)		\
{											\
	return static_cast <type##Aux*> (what);	\
}
#define ADD_CUT_AUX_DEFINITION(type) CUT_AUX_DEFINITION(type) ADD_AUX_DEFINITION(type)

ADD_CUT_AUX_DEFINITION(Node)
ADD_CUT_AUX_DEFINITION(Graph)
ADD_CUT_AUX_DEFINITION(Edge)


#undef ADD_CUT_AUX_DEFINITION
#undef ADD_AUX_DEFINITION
#undef CUT_AUX_DEFINITION
    

#endif
