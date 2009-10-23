/** 
* str_to_vector.h
* Parser can work in two options. It can
* take parameters from the command line
* or from the imput string. Functions 
* from this file are used in the second
* case. The imput string could be
* optimized in order to eliminate possible
* mistakes - commas and other separators
* could be removed. After that parameters
* are forwarded from the string to vector.
* Parser component
* Copyright 2009 MIPTVIS team 
*/

#ifndef _STR_TO_VECTOR_H
#define _STR_TO_VECTOR_H

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

class StrToVector
{
private:
	vector<string> v;

	static void removeSeparators( string& );
	static void removeSpacesInTheEnds( string& , bool );
	static void removeSpacesInTheMiddle( string& );
	void pushToVector( string );

public:
	vector<string> getv();
	void parse( string& );
	static void optimizeString( string& );
	void printVector( void);

	StrToVector( void){};
	~StrToVector( void){};
};


vector<string> StrToVector::getv()
{
	return v;
}

//Optimising the string and creating vector 'v'
void StrToVector::parse( string& s)
{
	
#if 0
	/*
	In order to delete separators from the imput string
	please uncommit the line that follows
	*/
	optimizeString(s);
#endif
		pushToVector( s);
}

/*
* Transforming the string to more appropriate view in order to eliminate
* possible mistakes with creation of vector 'v' that follows
*/
void StrToVector::optimizeString( string& s)
{
	removeSeparators( s);
	removeSpacesInTheEnds( s, 0);
	removeSpacesInTheEnds( s, 1);
	removeSpacesInTheMiddle( s);
}

//Displaying vector 'v'
void StrToVector::printVector( void)
{
	for ( unsigned int i = 0; i < v.size(); i++)
		cout << "v(" << i << "): " << v.at( i) << endl;
}

//Removing following separators: '\n', '\t', '\v', '\r', '\f', ','
void StrToVector::removeSeparators( string& s)
{
	for ( unsigned int i = 0; i < s.size() ; i++)
	{
		if ( ( isspace( s.at( i)) || s.at( i) == ',') 
			&& ( s.at( i) != ' ')) 
		{
			s.erase( i, 1);
		}
	}
}

//Removing spaces in the ends of the string
void StrToVector::removeSpacesInTheEnds( string& s, bool i)
{
	if ( i == 0)
	{
		while ( isspace( s.at( 0)))
		{
			s.erase( 0, 1);
		}
	}else
	{
		while ( isspace( s.at( s.size() - 1)))
		{
			s.erase( s.size() - 1, 1);
		}
	}
}

//Removing multiple spaces between words
void StrToVector::removeSpacesInTheMiddle( string& s)
{
	for ( unsigned int i = 0; i < s.size(); i++)
	{
		if ( isspace( s.at( i)))
		{
			while ( isspace( s.at( i + 1)))
			{
				s.erase( i + 1, 1);
			}
		}
	}
}

//Filling the vector 'v'
void StrToVector::pushToVector( string s)
{
	unsigned int l[ 2];
	for ( unsigned int i = 0; i < s.size(); i++)
	{
		if ( i == 0) l[ 0] = i;
		else if ( isspace( s.at( i)))
		{
			l[ 1] = i - 1;
			v.push_back( s.substr( l[ 0], l[ 1] - l[ 0] + 1));
			l[ 0] = i + 1;
		}else if ( i == ( s.size() - 1))
		{
			l[ 1] = i;
			v.push_back( s.substr( l[ 0], l[ 1] - l[ 0] + 1));
		}else 
			;
	}
}


#endif