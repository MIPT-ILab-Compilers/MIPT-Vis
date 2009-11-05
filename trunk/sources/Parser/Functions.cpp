/** 
* functions.cpp - Functoins, used for checking 
* the imput of Parser. This imput is to be
* the C or C++ string that consist of
* the following words:
* 1) text file (for instance 'some_file_name.txt') 
* - the result of ICC or GCC compiler;
* 2) xml file (for instance 'parsed_file.xml')
*  - specifies the filename after parsing;
* 3) Compiler name ('-icc' for Intel C++ compiler
* or '-gcc' for GNU Compiler Collection)
* If the spelling is correct, imput parameters
* are saved int the object of 'Parameters' class.
* Parser Component	
* Copyright 2009 MIPTVIS team 
*/

#include "parser.h"
#include "parameters.h"

int checkLen( int len)
{
	if ( len < 2 || len > 3) 
	{
		return 0;
	}else	//Mistake - illegal number of parameters
	{
		return 1;
	}
}

bool ifCompilerDefinedBefore( Parameters* p)
{
	if ( !( p->getCompiler()).compare( UNDEFINED)) 
	//Compiler is not defined yet
	{
		return false;			
	}else 
	//Compiler is already defined
	{
		return true;
	}					
}

bool ifThisIsCompiler( string s)
{
	if ( !( s.compare( ICC_COMPILER)) 
		 || !( s.compare( GCC_COMPILER))) 
	{
		return true;
	}else 
	{
		return false;
	}
}

bool ifTxtFileDefinedBefore( Parameters* p)
{
	if ( !( p->getTxtFile()).compare( UNDEFINED)) 
	//txt file is not defined yet
	{
		return false;			
	}else
	//txt file is already defined
	{
		return true;														
	}
}

bool ifThisIsTxtFile( string s)
{
	string s1;
	if ( s.size() >= 5)
	{
		s1 = s;
		s1.erase( s1.begin(), s1.end() - sizeof( ".txt") + 1);
		if ( !s1.compare( ".txt"))
		//this is txt file
		{
			return true;								
		}
	}else
		//this is not txt file
		;
	return false;														
}

bool ifXmlFileDefinedBefore( Parameters* p)
{
	if ( !( p->getXmlFile()).compare( UNDEFINED))
	//xml file is not defined yet
	{
		return false;			
	}else
	//xml file is already defined
	{
		return true;	
	}
}

bool ifThisIsXmlFile( string s)
{
	string s1;
	if ( s.size() >= 5)
	{
		s1 = s;
		s1.erase( s1.begin(), s1.end() - sizeof( ".xml") + 1);
		if ( !s1.compare( ".xml")) 
		//this is xml file
		{
			return true;								
		}
	}else
		//this is not xml file
		;
	return false;														
}
void makeXmlFileNameFromTxtFileName( Parameters* p)
{
	string s = p->getTxtFile();
	string s1( s);
	s1.erase( s1.end() - sizeof( ".txt") + 1, s1.end());
	s1.append( ".xml");
	p->setXmlFile( s1);
}

/*
* Main function of 'Functions.cpp' file.
* It checks parameters and saves it in
* the object of 'Parameters' class.
*/
int arrangeParameters( Parameters* param)
{
	string l;
	if ( !checkLen( param->getLen()))
	{
		cerr << "The number of parameters doesn't match."
			 << endl << endl;
		return 0;
	}else
	{
		while ( param->getLen() > 0)
		{	
			l = ( param->getNextParam());
			if ( ifCompilerDefinedBefore( param) 
				&& ifThisIsCompiler( l))
			{
				cerr << "Mistake - too many compilers. "
					"Please choose -icc or -gcc." << endl;
				return 0;
			}else if ( ifTxtFileDefinedBefore( param) 
				&& ifThisIsTxtFile( l))
			{
				cerr << "Mistake - too many txt files." << endl;
				return 0;
			}else if ( ifXmlFileDefinedBefore( param) 
				&& ifThisIsXmlFile( l))
			{
				cerr << "Mistake - too many xml files." << endl;
				return 0;
			}else if ( ifThisIsCompiler( l))
			{
				param->setCompiler( l);
			}else if ( ifThisIsTxtFile( l))
			{
				param->setTxtFile( l);
			}else if ( ifThisIsXmlFile( l)) 
			{
				param->setXmlFile( l );
			}else
			{
				cerr << "Mistake - unexpected filename or compiler." 
					 << endl;
				return 0;
			}
		}
		if ( !ifCompilerDefinedBefore( param))
		{
			cerr << "Mistake - undefined compiler." << endl;
			return 0;
		}else if ( !ifTxtFileDefinedBefore( param))
		{
			cerr << "Mistake - undefined file.txt" << endl;
			return 0;
		}else if ( !ifXmlFileDefinedBefore( param))
		{
			makeXmlFileNameFromTxtFileName( param);
		}
		return 1;
	}
}