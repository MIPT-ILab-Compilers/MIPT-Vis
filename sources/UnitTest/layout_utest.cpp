/**
 * File: layout_utest.cpp - Implementation of testing of layout library, 
 * internal graph placement component in MiptVis tool.
 * Copyright (C) 2009 MiptVis
 */

#include "utest_impl.h"

bool uTestLayout()
{
	GraphAux ga;
	
    char * file = _getcwd( NULL, 1024);
    strcat_s( file, 1024, "/test_graph.xml");

	ga.readFromXML (file);
	out ("\n============= testing Layout =============\nbefore ranking:\n");
	ga.debugPrint();
	ga.rank();
	out ("\nafter ranking:\n");
	ga.debugPrint();
	out ("\n=========== end of layout test ===========\n");
	return true;
}