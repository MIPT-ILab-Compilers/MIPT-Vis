#include "layout_iface.h"

//-----------------------------------------------------------------------------
EdgeAux::EdgeAux( GraphAux *graph_p, int _id, NodeAux* _pred, NodeAux* _succ):
    Edge( static_cast< Graph*>( graph_p), _id,  static_cast< Node*>( _pred),  static_cast< Node*>( _succ)),
	type (tree)
{}
//-----------------------------------------------------------------------------
void EdgeAux::debugPrint()
{
	Edge::debugPrint();
	switch (type)
	{
	case tree:
		out (" tree");
		break;
	case mesh:
		out (" loop");
		break;
	case back:
		out (" back");
		break;
	case forward:
		out (" forward");
		break;
	default:
		out (" wrong type");
	};
}
//-----------------------------------------------------------------------------