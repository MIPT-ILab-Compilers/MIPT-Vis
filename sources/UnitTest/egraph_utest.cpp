/**
 * File: egraph_utest.cpp - implementation and definition of functions 
 * to be tested in UnitTest, part of
 * Analysis library in MiptVis tool.
 * Copyright (C) 2009  AnalysisTeam
 */
#include "utest_impl.h"

static bool uTestMakeEntryExit( EGraph *egraph)
{
    egraph->makeGraphSingleExit();
    egraph->makeGraphSingleEntry();
    return true;
}

/**
 * Unit tests for EGraph functionalities
 */
bool uTestEGraph()
{
    EGraph egraph;
	vector<ENode *> enodes;
	/**
	 *Create nodes and edges
	 */
	printf("\nBegin EGraph Test\n");
	for ( int i =0; i<20; i++)
    {
        enodes.push_back( egraph.newNode());
        if ( i > 0)
        {
            egraph.newEdge( enodes[ i - 1], enodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            egraph.newEdge( enodes[ i - 2], enodes[ i]);
        }
    }

    if ( !uTestMakeEntryExit( &egraph))
        return false;
    
    egraph.newEdge( enodes[ 8], enodes[ 4]);
    delete enodes[ 8];
    egraph.debugPrint();
	printf("\nEnd Of EGraph Test\n");
	
	return true;
}
