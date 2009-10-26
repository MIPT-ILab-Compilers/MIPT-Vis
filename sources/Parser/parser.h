/** 
* parser.cpp - Implementation of Parser
* The program can handle files, compiled by
* Intel C++  or GNU Compilers
* Parser component
* Copyright 2009 MIPTVIS team 
*/

#ifndef _PARSER_H
#define _PARSER_H

#include <fstream>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>

#define ICC_COMPILER "-icc"
#define GCC_COMPILER "-gcc"
#define UNDEFINED "\n"



using namespace std;



class Parser
{
public:
	virtual bool parseFile( string file_name_xml) = 0;
	Parser( void){};
	virtual ~Parser( void){};
};



class ParserIcc : public Parser
{
public:
	bool parseFile( string file_name_txt)
	{
		std::cout << "icc parser is running..." << endl;
		return 0;
	}
	ParserIcc( void){};
public:
	~ParserIcc( void){};
};



class ParserGcc : public Parser
{
public:
	bool parseFile( string file_name_txt)
	{
		std::cout << "gcc parser is running..." << endl;
		return 0;
	}
	ParserGcc( void){};
public:
	~ParserGcc( void){};
};

#endif