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
 *  Check if graph is connected
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

/**
 * Function to find the node, which we can call entry for the subgraph.
 * Argument Enode *node is a pointer to node from the given subgraph.
 */
ENode* EGraph::findSubgraphEntry(ENode* node, Marker mrk)
{
	node->mark( mrk);
	EEdge* e = ( EEdge*)node->firstPred();
	ENode* pred = ( ENode*)e->pred();
	/**
     * as far as we know 1 node from subgraph, use backstepping to find the head cycle,
	 * which we can replace with a node, which dominates the others in subgraph.
     */
	while ( !pred->isMarked( mrk))
	{
		node = pred;
		e = ( EEdge*)node->firstPred();
		pred = ( ENode*)e->pred();
		node->mark( mrk);
	}
    return pred;
}
/**
 * Function to find the node, which we can call exit for the subgraph.
 * This function is very similar to findSubgraphEntry.
 * Argument Enode *node is a pointer to node from the given subgraph.
 */
ENode* EGraph::findSubgraphExit( ENode* node, Marker mrk)
{
	node->mark( mrk);
	EEdge* e = ( EEdge*)node->firstSucc();
	ENode* succ = ( ENode*)e->succ();
	/**
     * as far as we know 1 node from subgraph, find the ending cycle,
	 * which we can replace with a node, which postdominates the others in subgraph.
     */
	while ( !succ->isMarked( mrk))
	{
		node = succ;
		e = ( EEdge*)node->firstSucc();
		succ = ( ENode*)e->succ();
		node->mark( mrk);
	}
    return succ;
}

/**
 * Simple dfs, move down by successors, using recursion. Mark each visited node.
 */
void EGraph::visitAllSuccs( ENode *node, Marker m)
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

/**
 * Simple dfs, move up by predecessors, using recursion. Mark each visited node.
 */
void EGraph::visitAllPreds( ENode *node, Marker m)
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
 *  Create an entry for the graph.
 *  If the graph was disconnected, it becomes connected, and
 *  all nodes can be reached from the created entry.
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
    /**
	 * To find exceptions in graph, which need some other entry, do a DFS, using visitAllSuccs,
	 * then make new entry for that piece of graph.
	 * Do it until there isn't any nodes which cant be dominated by entry
	 */
	currententry = entrynode;
	isallmarked = false;
    firstUnmrkdNode = firstNode();
	while ( isallmarked != true)
    {
		/**
		 * Start dfs from some node "currententry" - the entry for previous subgraph, use marker m
		 */
        visitAllSuccs( currententry, m);
		isallmarked = true;
		/**
		 * Search through the list of nodes for unmarked ones. Since we get first of them 
		 * (it is not important which one), define it as a part of some unmarked subgraph.
		 */
        for (  n = firstUnmrkdNode; isNotNullP( n); n = (ENode* )n->nextNode())
		{
			if ( n->isMarked( m) != true) 
			{
				firstUnmrkdNode = n;
				isallmarked = false;
				break;
			}
		}
	    /**
	     * All unmarker nodes have no predcessors, so they are in cycles
	     * Lets determine the entry of each unmarked subgraph, use marker "mrk" to determine cycles
	     */
		Marker mrk = newMarker();
		if ( currententry->isMarked( m) != true)
		{
		    currententry = findSubgraphEntry( firstUnmrkdNode, mrk);
		    /**
		     * Connect the subgraph's 'entry' with the main entry
		     */
			newEdge( entrynode, currententry);
		}
		freeMarker( mrk);
    }
	freeMarker( m);
}

/**
 *  Create an exit for the graph.
 *  If the graph was disconnected, it becomes connected, and
 *  the exit can be reached from any node of the graph.
 */
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
    /**
	 * To find exceptions in graph, which need some other exit, do a DFS, using visitAllPreds,
	 * then make new exit for that piece of graph.
	 * Do it until there isn't any nodes which cant be postdominated by exit
	 */
	currentexit = exitnode;
	isallmarked = false;
    firstUnmrkdNode = firstNode();
	while ( isallmarked != true)
    {
		/**
		 * Start dfs from some node "currentexit" - founded early exit for last subgraph, use marker m
		 */
        visitAllPreds( currentexit, m);
		isallmarked = true;
		/**
		 * Search through the list of nodes for unmarked ones. Since we get first of them 
		 * (it is not important which one), define it as a part of some unmarked subgraph.
		 */
        for (  n = firstUnmrkdNode; isNotNullP( n); n = (ENode* )n->nextNode())
		{
			if ( n->isMarked( m) != true) 
			{
				firstUnmrkdNode = n;
				isallmarked = false;
				break;
			}
		}
	    /**
	     * All unmarker nodes have no successors, so they are in cycles
	     * Lets determine the exit of each unmarked subgraph, use marker "mrk" to determine cycles
	     */
		Marker mrk = newMarker();
		if ( currentexit->isMarked( m) != true)
		{
		    currentexit = findSubgraphExit( firstUnmrkdNode, mrk);
		    /**
		     * Connect the subgraph's 'exit' with the main exit
		     */
			newEdge( exitnode, currentexit);
		}
		freeMarker( mrk);
    }
	freeMarker( m);
}



