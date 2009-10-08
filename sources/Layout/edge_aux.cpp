#include "layout_iface.h"

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