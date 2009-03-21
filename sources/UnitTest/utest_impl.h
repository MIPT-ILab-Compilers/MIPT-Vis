/**
 * File: uTest_impl.h - Header for unit testing of libraries and tools of MiptVis
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef uTest_IMPL_H
#define uTest_IMPL_H

#include <stdio.h>
#include "../Graph/graph_iface.h"
#include "../Utils/utils_iface.h"

/**
 * Graph library unit testing
 */
bool uTestGraph();
bool uTestGraphXML();

#endif
