/**
 * File: egraph_utest.cpp - implementation and definition of functions 
 * to be tested in UnitTest, part of
 * Analysis library in MiptVis tool.
 * Copyright (C) 2009  AnalysisTeam
 */
#include "utest_impl.h"

static bool uTestMakeEntryExit(void)
{
	bool utest_passed = true;
    EGraph *egraph = new EGraph;
	vector<ENode *> enodes;
	printf("\n  Creating graph 1:\n");
	for ( int i =0; i<6; i++)
    {
        enodes.push_back( egraph->newNode());
	}
    egraph->newEdge( enodes[ 0], enodes[ 1]);
    egraph->newEdge( enodes[ 2], enodes[ 3]);
    egraph->newEdge( enodes[ 2], enodes[ 4]);
    egraph->newEdge( enodes[ 3], enodes[ 5]);
    egraph->newEdge( enodes[ 4], enodes[ 5]);

    egraph->debugPrint();

	printf("  Creating single-entry and single-exit\n");
	egraph->makeGraphSingleExit();
    egraph->makeGraphSingleEntry();

	printf("    Entry:\n");
	egraph->getEntry()->debugPrint();
	printf("    Exit:\n");
	egraph->getExit()->debugPrint();

	printf("  Final Graph:\n");
    egraph->debugPrint();

	/* The following code checks correctness only of makeGraphSingleEntry.
	 * TODO: It's needed to check if makeGraphSingleExit also works fine.
	 */
	if(!egraph->isGraphConnected()) 
		utest_passed = false;

	printf("  Deleting graph1...\n");
	delete egraph;
	enodes.clear();

	printf("\n  Creating graph 2:\n");
	egraph = new EGraph;
	for ( int i =0; i<4; i++)
    {
        enodes.push_back( egraph->newNode());
	}

    egraph->newEdge( enodes[ 0], enodes[ 1]);
    egraph->newEdge( enodes[ 1], enodes[ 0]);
    egraph->newEdge( enodes[ 2], enodes[ 3]);
    egraph->newEdge( enodes[ 3], enodes[ 2]);

    egraph->debugPrint();

	printf("  Creating single-entry and single-exit\n");
	egraph->makeGraphSingleExit();
    egraph->makeGraphSingleEntry();

	printf("    Entry:\n");
	egraph->getEntry()->debugPrint();
	printf("    Exit:\n");
	egraph->getExit()->debugPrint();

	printf("  Final Graph:\n");
    egraph->debugPrint();

	/* The following code checks correctness only of makeGraphSingleEntry.
	 * TODO: It's needed to check if makeGraphSingleExit also works fine.
	 */
	if(!egraph->isGraphConnected()) 
		utest_passed = false;

	printf("\nSingle-entry, single-exit unit test %s\n", 
		   utest_passed ? "successfully passed" : "failed");
    return utest_passed;
}

/**
 * Unit tests for EGraph functionalities
 */
bool uTestEGraph()
{
	printf("\nBegin EGraph Test\n");
    if ( !uTestMakeEntryExit())
        return false;
    
	printf("\nEnd Of EGraph Test\n");
	
	return true;
}
