/** 
* parser_main.cpp - Entry point of Parser 
* Parser component
* Copyright 2009 MIPTVIS team 
*/

#include "parser.h"
#include "parameters.h"
#include "functions.h"
#include "str_to_vector.h"

#define IMPUT_TYPE 1 // it could be 1 or 0

int main( int argc, char* argv[])
{
	Parser* P;
	Graph * gr;
	vector<string> v;
	StrToVector v1;
	bool imput_method;

#if IMPUT_TYPE
// Adding parameters to vector 'v' from cmd_line
	imput_method = 0;
	for ( int i = 0; i < argc; i++)
	{
		if ( i != 0)
		{
			string s( argv[i]);
			v.push_back( s);
		}
		else
			;
	}
#endif

#if !IMPUT_TYPE
//Adding parameters to vector 'v' from the string
	/*
	* In order to use imput parameters of parser
	* from the string but not from the command
	* line please uncommit the text that follows.
	*/
	imput_method = 1;
	string s = "1.txt 2.xml -gcc";
	v1.parse( s);
	cout << s << endl;
	v = v1.getv();
#endif

/* 
* Defining the object 'param' that could carry vector v, 
* sorted parameters of cmd_line and specific functions
*/
	Parameters* param = new Parameters( v);

// Sorting and checking parameters
	if ( !arrangeParameters( param))
	{
		help( imput_method);
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
		help( imput_method);
	}
	return 0;
}
