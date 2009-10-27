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

using namespace std;

class Parameters
{
private:
	string txt_file;
	string xml_file;
	string compiler;
	vector <string> v;
public:
//	vector v;
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
			string s = v[ i];
			cout << s << endl;
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

	Parameters( vector <string> v0): v( v0),
									txt_file( UNDEFINED),
									xml_file( UNDEFINED),
									compiler( UNDEFINED) {};
	~Parameters( void){};
};


#endif