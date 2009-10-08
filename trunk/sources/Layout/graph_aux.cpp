#include "layout_iface.h"


NodeAux* GraphAux::getFirstNode()
{
	return addAux (firstNode());
}
//-----------------------------------------------------------------------------
//Differs the types of all ehges
NodeAux* GraphAux::makeAcyclic()
{
	NodeAux* first = getFirstNode();
	if (first == 0) return 0;
	
	Marker unidir = newMarker();
	Marker ret = newMarker();

	makeAcyclicImp (first, unidir, ret);

	freeMarker (unidir);
	freeMarker (ret);

	return first;
}
//-----------------------------------------------------------------------------
//Internal implementation, one mustn't call from outside
void GraphAux::makeAcyclicImp (NodeAux* from, Marker passed, Marker ret)
{
	from->mark (passed);

	for (EdgeAux* cur = from->firstSucc(); !from->endOfSuccs();
		          cur = from->nextSucc())
		if (cur->succ() == from) cur->type = EdgeAux::mesh; //mesh
		else
		{
			if (cur->succ()->isMarked (passed))
			{
				if (cur->succ()->isMarked (ret))
					 cur->type = EdgeAux::forward;			//forward
				else cur->type = EdgeAux::back;				//back
			}
			else
			{
				cur->type = EdgeAux::tree;					//tree
				makeAcyclicImp (addAux (cur->succ()), passed, ret);
			}
		}

	from->mark (ret);
}
//-----------------------------------------------------------------------------
//Divide nodes to layers, using ranks
bool GraphAux::rank()
{
	NodeAux* root = makeAcyclic();
	if (root == 0) return false;
	
	rankImp (root, 0);
	
	return true;
}
//-----------------------------------------------------------------------------
//Internal implementation↓↓↓↓
void GraphAux::rankImp (NodeAux* from, int cur_rank)
{
	from->rang_priv = cur_rank;
	for (EdgeAux* cur = from->firstSucc(); !from->endOfSuccs();
		          cur = from->nextSucc())
	{
		if (cur->type == EdgeAux::tree) rankImp (addAux (cur->succ()), cur_rank + 1);
	}
}
//-----------------------------------------------------------------------------