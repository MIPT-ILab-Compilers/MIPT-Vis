/**
 * File: egraph.cpp - EGraph class implementation, part of
 * Analysis library in MiptVis tool.
 * Copyright (C) 2009  AnalysisTeam
 */
#include "analysis_impl.h"

/**
 *  Variables' initialization
 */
void EGraph::init()
{
    entrynode = NULL;
	exitnode = NULL;
    is_dom_tree_computed = false;
    is_pdom_tree_computed = false;
    is_cycle_tree_computed = false;   
	is_graph_connected = false;
}

EGraph::EGraph()
{
    init();
}

EGraph::~EGraph()
{

}


/**
 *  Shows if graph is connected
 */
bool EGraph::isGraphConnected()
{
    ENode *n;
	is_graph_connected = false;
	int i = 0;
    /** Create a marker to distinguish visited nodes from unvisited ones */
    Marker m = newMarker();

    /** 
     * Visit all nodes that doesn't have predecessors.
     * If the graph is connected, i must equals 1.
     */
	for (  n = firstNode(); isNotNullP( n); n = (ENode* )n->nextNode())
    {
        if( isNullP( n->firstPred()))
        {
            graphassert( !n->isMarked( m));
			i++;        
        }
    }   
	if ( i == 1) { is_graph_connected = true;}

    freeMarker( m);

	return is_graph_connected;
}
/**
 * Supportive subprograms
 */
ENode* underGraphEntry( ENode* node, Marker mrk)
{
	node->mark( mrk);
	EEdge* e = ( EEdge*)node->firstPred();
	ENode* pred = ( ENode*)e->pred();
	while ( !pred->isMarked( mrk))
	{
		node = pred;
		e = ( EEdge*)node->firstPred();
		pred = ( ENode*)e->pred();
		node->mark( mrk);
	}
    return pred;
}

ENode* underGraphExit( ENode* node, Marker mrk)
{
	node->mark( mrk);
	EEdge* e = ( EEdge*)node->firstSucc();
	ENode* succ = ( ENode*)e->succ();
	while ( !succ->isMarked( mrk))
	{
		node = succ;
		e = ( EEdge*)node->firstSucc();
		succ = ( ENode*)e->succ();
		node->mark( mrk);
	}
    return succ;
}

void visitAllSuccs( ENode *node, Marker m)
{
	EEdge *e;
	node->mark( m);
	for ( e = ( EEdge*)node->firstSucc(); !node->endOfSuccs(); e = ( EEdge*)node->nextSucc())
    {
        ENode* succ = ( ENode*)e->succ();
        if ( !succ->isMarked( m))
        {
            visitAllSuccs( succ, m);
        }
    }
}

void visitAllPreds( ENode *node, Marker m)
{
	EEdge *e;
	node->mark( m);
	for ( e = ( EEdge*)node->firstPred(); !node->endOfPreds(); e = ( EEdge*)node->nextPred())
    {
        ENode* pred = ( ENode*)e->pred();
        if ( !pred->isMarked( m))
        {
            visitAllPreds( pred, m);
        }
    }
}
/**
 *  Create fake nodes entry and exit.
 */
void EGraph::makeGraphSingleEntry()
{
    ENode *n;
	ENode *currententry;
	ENode* firstUnmrkdNode;
    entrynode = newNode();
    Marker m = newMarker();
	bool isallmarked;
    /** Create a marker to distinguish visited nodes from unvisited ones */
    /** 
     * Visit all nodes that doesn't have predecessors and connect them with entry.
     */
    for (  n = firstNode(); isNotNullP( n); n = (ENode* )n->nextNode())
    {
        if ( isNullP( n->firstPred()) && ( n != entrynode)) 
        {
            newEdge( entrynode, n);
        }
    }   
	currententry = entrynode;
	isallmarked = false;
    firstUnmrkdNode = firstNode();
	while ( isallmarked != true)
    {
        visitAllSuccs( currententry, m);
		isallmarked = true;
        for (  n = firstUnmrkdNode; isNotNullP( n); n = (ENode* )n->nextNode())
		{
			if ( n->isMarked( m) != true) 
			{
				firstUnmrkdNode = n;
				isallmarked = false;
				break;
			}
		}
		Marker mrk = newMarker();
		if ( currententry->isMarked( m) != true)
		{
		    currententry = underGraphEntry( firstUnmrkdNode, mrk);
		}
		freeMarker( mrk);
    }
	freeMarker( m);
}

void EGraph::makeGraphSingleExit()
{
    ENode *n;   
    exitnode = newNode();
	ENode *currentexit;
	ENode* firstUnmrkdNode;
    Marker m = newMarker();
	bool isallmarked;
    /** Create a marker to distinguish visited nodes from unvisited ones */
    /** 
     * Visit all nodes that doesn't have successors and connect them with exit.
     */
    for (  n = firstNode(); isNotNullP( n); n = (ENode* )n->nextNode())
    {
        if ( isNullP( n->firstSucc()) && ( n != exitnode))
        {
            newEdge( n, exitnode);
        }
    } 
	currentexit = exitnode;
	isallmarked = false;
    firstUnmrkdNode = firstNode();
	while ( isallmarked != true)
    {
        visitAllPreds( currentexit, m);
		isallmarked = true;
        for (  n = firstUnmrkdNode; isNotNullP( n); n = (ENode* )n->nextNode())
		{
			if ( n->isMarked( m) != true) 
			{
				firstUnmrkdNode = n;
				isallmarked = false;
				break;
			}
		}
		Marker mrk = newMarker();
		if ( currentexit->isMarked( m) != true)
		{
		    currentexit = underGraphExit( firstUnmrkdNode, mrk);
		}
		freeMarker( mrk);
    }
	freeMarker( m);
}
