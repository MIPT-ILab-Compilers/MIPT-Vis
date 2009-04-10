/**
 * File: uTest_impl.h - Header for unit testing of libraries and tools of MiptVis
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef UTEST_IMPL_H
#define UTEST_IMPL_H

#include <stdio.h>
#include "../Graph/graph_iface.h"
#include "../Utils/utils_iface.h"
#include "../GUI/gui_iface.h"

/**
 * Graph library unit testing
 */
bool uTestGraph();
bool uTestChain( int argc, char * argv[]);

/**
 * Utils library unit testing
 */
bool uTestList();

#endif
