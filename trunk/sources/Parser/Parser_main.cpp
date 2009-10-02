// Parser_cmd.cpp : Defines the entry point for the console application.

#include "Parser.h"
#include "Parameters.h"
#include "Functions.h"



int main(int argc, char* argv[])
{
	Parser* P;
	vector<string> v;

// Adding parameters to vector 'v'
	for( int i = 0; i < argc; i++){								
		string s(argv[i]);
		v.push_back(s);
	}
// Defining the object 'param' that could carry vector v, sorted parameters of cmd_line and specific functions
	Parameters* param = new Parameters(v);

// Sorting and checking parameters
	if( !arrange_parameters(param) )
		help();

//Parsing
	else if( !(param->get_compiler().compare(ICC_COMPILER)) ){	
		P = new Parser_icc();
		P->parseFile(param->get_xml_file());
		cout << "Parsed file is saved as " << param->get_xml_file() << endl;
	}
	else if( !(param->get_compiler().compare(GCC_COMPILER)) ){
		P = new Parser_gcc();
		P->parseFile(param->get_xml_file());
		cout << "Parsed file is saved as " << param->get_xml_file() << endl;
	}
	else{
		cout << "Sorry, mistake has occured. Please try again." << endl;
//		param ->print_cmd_line();
		help();
	}
	return 0;
}

