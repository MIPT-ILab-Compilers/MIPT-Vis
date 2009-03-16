/**
 * File: graph_utest.cpp - Implementation of testing of Graph library, 
 * internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"
#include <list>
#include <vector>
#include "direct.h"
#include <string.h>

using namespace std;

/*** Complex testing for markers */
static bool UTestDFS( Graph *graph)
{
	NodeListItem* nodes = graph->DFS();
    return true;
}

 /**
  * TODO: Check graph's data structures being consistent with node and edge functionality
  */
static bool UTestGraphOwn()
{
    return true;
}
/**
 * TODO: Check consistency of Node and Edge classes interoperation
 */
static bool UTestNodeEdge()
{
    /**
     * TODO: Check that node and edge remain correct after basic edge/node creation/deletion
     */
    /**
     * 
     */
    return true;
}

/**
 * Check marker functionality
 */
static bool UTestMarkers()
{
    Graph graph;
    Node *dummy = graph.NewNode();
    delete dummy;
    Node *pred = graph.NewNode();
    Node *succ = graph.NewNode();
    Edge *edge = graph.NewEdge( pred, succ);
    Marker m = graph.NewMarker();
    Marker m2 = graph.NewMarker();

    Marker m_array[ MAX_GRAPH_MARKERS];
    
    Assert( !pred->IsMarked( m));
    Assert( !succ->IsMarked( m));
    Assert( !edge->IsMarked( m));
    Assert( !pred->IsMarked( m2));
    
    pred->Mark( m);
    succ->Mark( m);
    edge->Mark( m);
    edge->Mark( m2);

    Assert( pred->IsMarked( m));
    Assert( succ->IsMarked( m));
    Assert( edge->IsMarked( m));
    Assert( edge->IsMarked( m2));
    edge->Unmark( m);

    /** Check that different markers have different behaviour */
    Assert( edge->IsMarked( m2));
    Assert( !edge->IsMarked( m));
    
    graph.FreeMarker( m);
    graph.FreeMarker( m2);
    
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        m_array [ i] = graph.NewMarker();
    }
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        graph.FreeMarker( m_array[ i]);
    }
    m = graph.NewMarker();
    graph.FreeMarker( m);
    
    Node *n;
    for (  n = graph.GetFirstNode(); !graph.EndOfNodes(); n = graph.GetNextNode())
    {
        delete n;
    }
    return true;
}

/**
 * Unit tests for Graph library
 */
bool UTestGraph()
{
    Graph graph;

    /** 
     *  Check basic operation of graph library
     */
    vector<Node *> nodes;

    /** Create nodes and edges */
    for ( int i =0; i<20; i++)
    {
        nodes.push_back( graph.NewNode());
        if ( i > 0)
        {
            graph.NewEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            graph.NewEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    graph.NewEdge( nodes[ 8], nodes[ 4]);
    delete nodes[ 8];
    graph.DebugPrint();
    
    if ( !UTestDFS( &graph))
        return false;
    /**
     * Check graph's data structures consistency
     */
     if ( !UTestGraphOwn())
         return false;
    /**
     * Check node-edge consistency
     */
    if ( !UTestNodeEdge())
         return false;

    /**
     * Check markers
     */
    if ( !UTestMarkers())
        return false;

    //Assert<Error>( 0);
    return true;
}

bool UTestReadGraph()
{
	char * file = _getcwd( NULL, 1024);
	strcat_s( file, 1024,"/test_graph.xml");
	Graph * graph = new Graph( file);
	if ( graph == NULL) return false;
	graph->DebugPrint();

	return true;
}