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
public:
	~Parser( void){};
};



class ParserIcc : public Parser
{
public:
	bool parseFile( string file_name_xml)
	{
		ofstream outfile( file_name_xml.c_str());
		outfile << "This will be Graph_icc" << endl;
		cout << "icc parser" << endl;
		return 0;
	}
	ParserIcc( void){};
public:
	~ParserIcc( void){};
};



class ParserGcc : public Parser
{
public:
	bool parseFile( string file_name_xml)
	{
		ofstream outfile( file_name_xml.c_str());
		outfile << "This will be Graph_gcc" << endl;
		cout << "gcc parser" << endl;
		return 0;
	}
	ParserGcc( void){};
public:
	~ParserGcc( void){};
};

#endif