/**
 * File: UnitTest/main.cpp - Implementation and entry point for unit testing of MiptVis
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"

int main(int argc, char **argv)
{
	//Test graph package
    if ( !uTestGraph())
        return -1;

	if ( !TestforBFS())
		return -1;
    if ( !uTestChain( argc, argv))
		return -1;

    return 0;	
}