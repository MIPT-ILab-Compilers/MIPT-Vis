#ifndef _PARSER_H
#define _PARSER_H

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
	virtual bool parseFile( string file_name_txt) = 0;
	Parser(void){};
public:
	virtual ~Parser(void){};
};



class Parser_icc : public Parser
{
public:
	bool parseFile( string file_name_txt)
	{
		cout << "icc parser is working..." << "input file: " << file_name_txt << endl;
		return 0;
	}
	Parser_icc(void){};
public:
	~Parser_icc(void){};
};



class Parser_gcc : public Parser
{
public:
	bool parseFile( string file_name_txt)
	{
		cout << "gcc parser is working..." << "input file: " << file_name_txt << endl;
		return 0;
	}
	Parser_gcc(void){};
public:
	~Parser_gcc(void){};
};

#endif