/** 
* parser_main.cpp - Entry point of Parser 
* Parser component
* Copyright 2009 MIPTVIS team 
*/

#include "parser.h"
#include "parameters.h"
#include "functions.h"

int main( int argc, char* argv[])
{
	Parser* P;
	Graph * gr;
/* 
* Defining the object 'param' that could carry vector v, 
* sorted parameters of cmd_line and specific functions
*/
	Parameters* param = new Parameters();
	param->parseParams( argc, argv);

// Sorting and checking parameters
	if ( !arrangeParameters( param))
	{
		param->help();
	}else if ( !( param->getCompiler().compare( ICC_COMPILER)))
	//Parsing by ICC parser
	{	
		P = new Icc_parser();

		try
		{
			P->parseFile( param->getTxtFile());
			gr = P->getGraph();
		}
		catch( exSomething & ex)
		{
			ex.wtf( std::cerr);
			exit( 2);
		}

		gr->writeToXML( param->getXmlFile().c_str());
	}else if ( !( param->getCompiler().compare( GCC_COMPILER)) )
	//Parsing by GCC parser
	{
		P = new Gcc_parser();

		try
		{
			P->parseFile( param->getTxtFile());
			gr = P->getGraph();
		}
		catch( exSomething & ex)
		{
			ex.wtf( std::cerr);
			exit( 2);
		}

		gr->writeToXML( param->getXmlFile().c_str());

	}else
	{
		std::cout << "Sorry, mistake has occured. Please try again." 
			<< std::endl;
		param->help();
	}
    delete P;
    delete param;
	delete gr;
	return 0;
}
