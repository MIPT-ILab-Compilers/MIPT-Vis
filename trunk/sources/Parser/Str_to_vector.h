#ifndef _STR_TO_VECTOR_H
#define _STR_TO_VECTOR_H

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

class Str_to_vector: public string, vector <string>
{
private:
	vector<string> v;

public:
	vector<string> getv(){return v;}

	//Optimising the string and creating vector 'v'
	void parse(string& s)
	{
		//In order to delete separators from the imput string please uncommit the line that follows.
	/*	optimize_string(s);*/
		push_to_vector(s);
	}

	//Transforming the string to more appropriate view in order to eliminate
	//possible mistakes with creation of vector 'v' that follows
	static void optimize_string(string& s)
	{
		remove_separators(s);
		remove_spaces_in_the_ends(s, 0);
		remove_spaces_in_the_ends(s, 1);
		remove_spaces_in_the_middle(s);
	}
	
	//Displaying vector 'v'
	void print(void)
	{
		for(unsigned int i = 0; i < v.size(); i++)	cout << "v(" << i << "): " << v.at(i) << endl;
	}

private:

	//Removing following separators: '\n', '\t', '\v', '\r', '\f', ','
	static void remove_separators(string& s)
	{
		for( unsigned int i = 0; i < s.size() ; i++){
			if( (isspace(s.at(i)) || s.at(i) == ',') && (s.at(i) != ' ') )s.erase(i,1);
		}
	}

	//Removing spaces in the ends of the string
	static void remove_spaces_in_the_ends(string& s, bool i)
	{
		if(i == 0)
			while( isspace(s.at(0)) ) s.erase(0,1);
		else{
			while( isspace(s.at(s.size()-1)) ) s.erase( s.size()-1,1 );
		}
	}

	//Removing multiple spaces between words
	static void remove_spaces_in_the_middle(string& s)
	{
		for(unsigned int i = 0; i < s.size(); i++){
			if( isspace(s.at(i)) ){
				while( isspace(s.at(i+1)) ) s.erase(i+1,1);
			}
		}
	}

	//Filling the vector 'v'
	void push_to_vector(string s)
	{
		unsigned int l[2];
		for( unsigned int i = 0; i < s.size(); i++ ){
			if( i == 0 ) l[0] = i;
			else if( isspace(s.at(i)) ){
				l[1] = i - 1;
				v.push_back( s.substr(l[0], l[1] - l[0] + 1) );
				l[0] = i + 1;
			}			
			else if( i == (s.size() - 1) ){
				l[1] = i;
				v.push_back( s.substr(l[0], l[1] - l[0] + 1) );
			}
			else ;
		}
	}

public:
	Str_to_vector(void){};
	~Str_to_vector(void){};
};

#endif