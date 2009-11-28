/** 
* functions.h - Prototypes of externally visible
* functions in functions.cpp
* Parser Component	
* Copyright 2009 MIPTVIS team 
*/

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "parameters.h"

void help( bool imput_method);
int arrangeParameters( Parameters* param);
bool convertDumpToXML( char * file)
{
	return true;
}

#endif