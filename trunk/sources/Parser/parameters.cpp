
#include "parameters.h"

//Removing following separators: '\n', '\t', '\v', '\r', '\f', ','
void Parameters::removeSeparators( string & s)
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

//Removing multiple spaces between words
void Parameters::removeSpacesInTheMiddle( string & s)
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

//Removing spaces in the ends of the string
void Parameters::removeSpacesInTheEnds( string & s)
{
	while ( isspace( s.at( 0)))
	{
		s.erase( 0, 1);
	}
	while ( isspace( s.at( s.size() - 1)))
	{
		s.erase( s.size() - 1, 1);
	}
}

/*
* Transforming the string to more appropriate view in order to eliminate
* possible mistakes with creation of vector 'v' that follows
*/
void Parameters::optimizeString( string & s)
{
	removeSeparators( s);
	removeSpacesInTheEnds( s);
	removeSpacesInTheMiddle( s);
}

//Filling the vector 'v'
void Parameters::pushToVector( string s)
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

void Parameters::help()
{
	if ( input_method == SEPARATE_TOOL )
	{
		cout << "Usage: \"parser.exe file_name.txt param1 param2(optional)\"" << endl;
	}else
	{
		cout << "Usage: \"file_name.txt param1 param2(optional)\"" << endl;
	}
	cout << "Param1: " << ICC_COMPILER << " or " << GCC_COMPILER << endl;
	cout << "	" << ICC_COMPILER << " -Use ICC compiler" << endl;
	cout << "	" << GCC_COMPILER << " -Use GCC compiler" << endl;
	cout << "Param2(Optional): parsed_file_name.xml" << endl;
	cout << "	Parsed file will be saved as 'parsed_file_name.xml'" << endl;
	cout << "	On default 'file_name.xml' will be created." << endl;
}