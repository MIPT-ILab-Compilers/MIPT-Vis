/** 
* File: graph_aux.cpp - Auxiliary graph class implementation, part of
* layout library, internal graph placement component in MiptVis tool.
* Copyright (C) 2009 MiptVis
*/

#include "layout_iface.h"


//-----------------------------------------------------------------------------
/**
* Ranking algorithm. Divide nodes to layers
*/
bool GraphAux::rank()
{
	NodeAux* root = makeAcyclic();
	if (root == 0) return false;
	
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
	return true;
}
//-----------------------------------------------------------------------------
/**
* Simple position, only for test ranking
*/
bool GraphAux::position()
{
	arrangeVertical();
	arrangeHorisontal();
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
	if (!rank())			return false;
	if (!ordering())		return false;
	if (!position())		return false;
	if (!make_splines())	return false;
	return true;
}
//-----------------------------------------------------------------------------
/**
* Finding a node like root//!!! if there are several roots, it returns the first one
*/
NodeAux* GraphAux::findRoot()
{
	for (Node* cur = firstNode(); cur != 0;cur = cur->nextNode())
		if (cur->firstPred() == 0)
			return addAux (cur);
	return addAux (firstNode());//if there are not roots in graph, return any node
}
//-----------------------------------------------------------------------------
/**
* Make graph acyclic and set to all edges corresponding type
*/
NodeAux* GraphAux::makeAcyclic()
{
	NodeAux* first = findRoot();
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
	for(EdgeAux* iter = firstEdge(); iter != NULL; iter = iter->nextEdge())
	{
		int rang_pred = iter->pred()->rang_priv, rang_succ = iter->succ()->rang_priv;
		if( abs(rang_pred - rang_succ) > 1)
		{
			NodeAux* n = insertNodeOnEdge( iter);
			n->setReal (false);
			if( rang_pred > rang_succ)
				n->rang_priv = rang_pred - 1;
			else
				n->rang_priv = rang_pred + 1;
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
 * Arrnages nodes' vertical positions by their ranks
 */
void GraphAux::arrangeHorisontal()
{
	int startx = 0;
	int stepx = 200;

	int max_rank = 0;
	for (NodeAux* iter = addAux(firstNode()); iter != 0; iter = addAux(iter->nextNode()))
		max_rank = max(iter->rang(), max_rank);

	max_rank++;

	int *num_in_rank = new int[max_rank];
	for (int i = 0; i < max_rank; ++i)
		num_in_rank[i] = 0;

	for (NodeAux* iter = addAux(firstNode()); iter != 0; iter = addAux(iter->nextNode()))
	{
		int pos = (num_in_rank[iter->rang()]++)*stepx + startx;
		iter->setX (pos);
	}

	//delete []num_in_rank;//!!!Не удаляется почему-то, TODO
}
//-----------------------------------------------------------------------------
void GraphAux::applayPositions()
{
	for (NodeAux* iter = addAux(firstNode()); iter != 0;
		          iter = addAux(iter->nextNode()))
	{
		iter->commitPos (iter->x(), iter->y());
		QString label;
		label.sprintf ("r=%d,o=%d", iter->rang(), iter->posAux());
		if (iter->real())
			iter->superscribe (Qt::gray, label);
		else
			iter->superscribe (Qt::white, label);
	}
}
//-----------------------------------------------------------------------------