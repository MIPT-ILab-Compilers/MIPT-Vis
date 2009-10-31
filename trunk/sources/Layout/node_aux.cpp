#include "layout_iface.h"

//-----------------------------------------------------------------------------
void NodeAux::debugPrint()
{
	Node::debugPrint();
	out (" rang: %i; pos: %i; real:%i", rang_priv, pos_priv, (int)real());
}
//-----------------------------------------------------------------------------
