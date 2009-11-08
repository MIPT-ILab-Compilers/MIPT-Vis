/** 
* parameters.cpp 
* Fields in 'Parameters' Class is used for convinient
* sorting of imput parameters - compiler type and
* file names. 
* Parser component
* Copyright 2009 MIPTVIS team 
*/

#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include <iostream>
#include <vector>

using namespace std;

#define ICC_COMPILER "-icc"
#define GCC_COMPILER "-gcc"
#define UNDEFINED "\n"

enum InputMethod { UNKNOWN_TOOL, SEPARATE_TOOL, INTEGRATED_TOOL };

class Parameters
{
private:
    InputMethod input_method;
	string txt_file;
	string xml_file;
	string compiler;
	vector <string> v;

    void removeSeparators( string& );
    void removeSpacesInTheMiddle( string& );
	void removeSpacesInTheEnds( string& );

	void pushToVector( string );
    void optimizeString( string& );
public:
	void help();

	void setTxtFile( string s)
	{
		txt_file = s;
	}

	void setXmlFile( string s)
	{
		xml_file = s;
	}

	void setCompiler( string s)
	{
		compiler = s;
	}

	string getTxtFile( void)
	{
		return txt_file;
	}

	string getXmlFile( void)
	{
		return xml_file;
	}

	string getCompiler( void)
	{
		return compiler;
	}

	void printCmdLine()
	{
		for( unsigned int i = 0; i < v.size(); i++)
		{
			cout << v[i].c_str() << endl;
		}
		cout << endl;
	}

	string getNextParam()
	{
			string s = v.back();
			v.pop_back();
			return s;
	}

	int getLen()
	{
		return ( int) v.size();
	}

	Parameters(): txt_file( UNDEFINED),
				  xml_file( UNDEFINED),
			      compiler( UNDEFINED),
                  input_method( UNKNOWN_TOOL){};
    void parseParams( int argc, char* argv[])
	{
        input_method = SEPARATE_TOOL;
		for ( int i = 1; i < argc; i++)
		{
			string s( argv[i]);
			v.push_back( s);
		}
	}
    void parseParams( string & s)
	{
        input_method = INTEGRATED_TOOL;
        optimizeString( s);
        pushToVector( s);
	}
	~Parameters( void){};
};


#endif