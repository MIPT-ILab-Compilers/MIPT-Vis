/** 
* File: graph_aux.cpp - Auxiliary graph class implementation, part of
* layout library, internal graph placement component in MiptVis tool.
* Copyright (C) 2009 MiptVis
*/

#include <Qt/QQueue.h>
#include <Qt/QStack.h>
#include "layout_iface.h"


//-----------------------------------------------------------------------------
/**
* Ranking algorithm. Divide nodes to layers
*/
bool GraphAux::rank()
{
	NodeAux* root = makeAcyclic();
	if (root == 0) return false;

	clearRanks();
	Marker passed = newMarker();
	rankImp (root, 0, passed);
	freeMarker (passed);

	return true;
}
//-----------------------------------------------------------------------------
/**
* Here must be an implementation of ordering in layers
*/
bool GraphAux::ordering()
{
	addVirtualChains();

	max_rank = 0;
	for (NodeAux* iter = addAux(firstNode()); iter != 0; iter = addAux(iter->nextNode()))
		max_rank = max(iter->rang(), max_rank);

	Rank rank( this);

	rank.doOrderAll();

	return true;
}
//-----------------------------------------------------------------------------
/**
* Simple position, only for test ranking
*/
bool GraphAux::position()
{
	arrangeVertical();
	applayPositions();
	return true;
}
//-----------------------------------------------------------------------------
/**
* Here might be laying a edges.
*/
bool GraphAux::make_splines()
{
	return true;
}
//-----------------------------------------------------------------------------
/**
* The main function of our component
*/
bool GraphAux::doLayout()
{
	delVirtualNodes();
	if (!rank())			return false;
	if (!ordering())		return false;
	if (!position())		return false;
	if (!make_splines())	return false;
	if (!applayLayout())	return false;
	
	return true;
}
//-----------------------------------------------------------------------------
/**
* Deletes virtual roots, translate nodes, returns false if there are any
* virtuals state in graph
*/
bool GraphAux::delVirtualNodes()
{
	bool rez = true;
	Node* to_detach = 0;
	for (Node* cur = firstNode(); cur != 0; cur = cur->nextNode())
	{
		if (to_detach)
		{
			removeNode (to_detach);
			//delete to_detach;
			to_detach = 0;
		}

		if (!addAux(cur)->real())
		{
			int num_preds = 0;
			int num_succs = 0;
			Edge* iter;
			ForEdges(cur, iter, Succ) ++num_succs;
			ForEdges(cur, iter, Pred) ++num_preds;

			if (num_preds == 0 && num_succs == 0) to_detach = cur; //lonely node
			else if (num_preds == 0)				//virtual root
			{
//				ForEdges(cur, iter, Succ) removeEdge (iter);
				to_detach = cur;
			}
			else if (num_preds == 1 && num_succs == 1)//translation node(edge point)
			{
				Edge* one = cur->firstSucc();
				Edge* two = cur->firstPred();
				one->setPred (two->pred());
//				removeEdge (two);
				to_detach = cur;
			}
			else rez = false;//it node can't be deleted
		}
	}
	if (to_detach)//for the last
	{
		removeNode (to_detach);
//		delete to_detach;
		to_detach = 0;
	}
	return rez;
}
//-----------------------------------------------------------------------------
/**
* Finding a node like root
*/
NodeAux* GraphAux::findRoot()
{
	QList <Node*> roots;
	for (Node* cur = firstNode(); cur != 0;cur = cur->nextNode())
		if (cur->firstPred() == 0)
			roots.push_back (cur);

	int num_reach = 0;
	Marker reachable = newMarker();
	foreach(Node* cur_root, roots)
	{
		num_reach += markReachable(cur_root, reachable);
	}
	while (num_reach != getNodeCount())//these roots are not grab all graph
	{
		for (Node* cur = firstNode(); cur != 0;cur = cur->nextNode())
			if (!cur->isMarked(reachable))
			{
				roots.push_back (cur);
				break;
			}
		num_reach += markReachable (roots.last(), reachable);//!!!&&& do not forget to correct
	}
	freeMarker (reachable);

	if (roots.size() >  1) return makeVirtualRoot (roots);
	if (roots.size() == 1) return addAux (roots.back());
						   return 0; //it's impossible
}
//-----------------------------------------------------------------------------
/**
* Marks all nodes, reachable from root by by_what, and returns theis number
*/
int GraphAux::markReachable (Node* root, Marker by_what)
{
	root->mark (by_what);
	int number_marked = 0;
	QQueue<Node*> to_process;
	to_process.enqueue (root);

	while (to_process.size() != 0)
	{
		++number_marked;
		Node* cur = to_process.dequeue();
		for (Edge* csucc = cur->firstSucc(); csucc != 0; csucc = csucc->nextSucc())
			if (!csucc->succ()->isMarked (by_what))
			{
				csucc->succ()->mark (by_what);
				to_process.enqueue (csucc->succ());
			}
	}
	return number_marked;
}
//-----------------------------------------------------------------------------
/**
* Adds virtual node - predesessor of all roots, 
*/
NodeAux* GraphAux::makeVirtualRoot (QList <Node*>& roots)
{
	if (roots.size() < 1) return 0;

	NodeAux* root = newNode();
	root->setReal (false);

	foreach(Node* iter, roots)
	{
		newEdge (root, addAux(iter));
	}
	return root;
}
//-----------------------------------------------------------------------------
/**
* Make graph acyclic and set to all edges corresponding type
*/
NodeAux* GraphAux::makeAcyclic()
{
	NodeAux* first = findRoot();
	if (first == 0) return 0;
	
	Marker passed = newMarker();
	Marker ret = newMarker();

	QStack<NodeAux*> to_process;
	to_process.push (first);

	while (to_process.size() > 0)
	{
		NodeAux* from = to_process.pop();
		
		if (from->isMarked (passed))//the second pass of node
		{
			from->mark (ret);
			continue;
		}
		from->mark (passed);

		to_process.push(from);//Need to pass this repeatedly, to mark as returned

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
					to_process.push (cur->succ());//process childs
				}
			}
	}


	freeMarker (passed);
	freeMarker (ret);

	return first;
}
//-----------------------------------------------------------------------------
/*
 * Internal implementation, ranks nodes by the longest path for it
 */
void GraphAux::clearRanks()
{
	for (Node* iter = firstNode(); iter != 0; iter = iter->nextNode())
		addAux(iter)->setRang (0);
}
//-----------------------------------------------------------------------------
/*
 * Internal implementation, ranks nodes by the longest path for it
 */
void GraphAux::rankImp (NodeAux* from, int cur_rank, Marker passed)
{
	if (from->rang_priv <= cur_rank)//Choose maximal lenght
		from->rang_priv =  cur_rank;

	if (passedAllPred (from, passed))
		passAllSucc (from, cur_rank, passed);//Wait for pass all previous nodes
}
//-----------------------------------------------------------------------------
/*
 * Make ranking for all successors
 */
void GraphAux::passAllSucc (NodeAux* from, int cur_rank, Marker passed)
{
	EdgeAux* cur;

	ForEdges(from, cur, Succ)
	{
		if (cur->ahead())
		{
			cur->mark (passed);
			rankImp (addAux (cur->succ()), cur_rank + 1, passed);//climb to a max-lenght path
		}
	}	
	ForEdges(from, cur, Pred)
	{
		if (cur->backward())
		{
			cur->mark (passed);
			rankImp (addAux (cur->pred()), cur_rank + 1, passed);//climb to a max-lenght path
		}
	}
}
//-----------------------------------------------------------------------------
/*
 * returns true only when all "pred" edges of from are marked "passed"
 */
bool GraphAux::passedAllPred (NodeAux* from, Marker passed)
{
	EdgeAux* cur;

	ForEdges(from, cur, Succ)
		if (cur->backward() && !cur->isMarked (passed))
			return false;

	ForEdges(from, cur, Pred)
		if (cur->ahead() && !cur->isMarked (passed))
			return false;
	return true;//all incoming edges are processed
}
//-----------------------------------------------------------------------------
void GraphAux::addVirtualChains()
{
	bool added = true;
	while (added)// number of edges increases
	{
		added = false;
		for (EdgeAux* iter = firstEdge(); iter != NULL; iter = iter->nextEdge())
		{
			int rang_pred = iter->pred()->rang_priv, rang_succ = iter->succ()->rang_priv;
			if( abs(rang_pred - rang_succ) > 1)
			{
				added = true;
				NodeAux* n = insertNodeOnEdge( iter);
				n->setReal (false);
				n->setWidth(20);
				if( rang_pred > rang_succ)
					n->rang_priv = rang_pred - 1;
				else
					n->rang_priv = rang_pred + 1;
			}
		}
	}
}
//-----------------------------------------------------------------------------
/*
 * Arrnages nodes' vertical positions by their ranks
 */
void GraphAux::arrangeVertical()
{
	int starty = 0;
	int stepy = 70;

	for (NodeAux* iter = addAux(firstNode()); iter != 0; iter = addAux(iter->nextNode()))
	{
		iter->setY (iter->rang()*stepy + starty);
	}
}
//-----------------------------------------------------------------------------
/*
 * Arrnages nodes' horisontal positions by their poses
 */
void GraphAux::arrangeHorisontal()
{
	int startx = 0;
	int stepx = 120;

	for (NodeAux* iter = addAux(firstNode()); iter != 0; iter = addAux(iter->nextNode()))
	{
		iter->setX (iter->posAux()*stepx + startx);
	}
}
//-----------------------------------------------------------------------------
void GraphAux::applayPositions()
{
	for (NodeAux* iter = addAux(firstNode()); iter != 0;
		          iter = addAux(iter->nextNode()))
	{
		iter->commitPos (iter->x(), iter->y());
		if (!iter->real())
			iter->superscribe (Qt::gray, "unreal");
	}
}
//-----------------------------------------------------------------------------