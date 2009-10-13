// Parser_cmd.cpp : Defines the entry point for the console application.

#include "Parser.h"
#include "Parameters.h"
#include "Functions.h"
#include "Str_to_vector.h"



int main(int argc, char* argv[])
{
	Parser* P;
	Str_to_vector v1;

// Adding parameters to vector 'v' from cmd_line or string
	v1.parse( argc, argv);

// Defining the object 'param' that could carry vector v, sorted parameters of cmd_line and specific functions
	Parameters* param = new Parameters(v1.getv());

// Sorting and checking parameters
	if( !arrange_parameters(param) )
		help( v1.getImputMethod());

//Parsing
	else if( !(param->get_compiler().compare(ICC_COMPILER)) ){	
		P = new Parser_icc();
		P->parseFile( param->get_txt_file());
		cout << "output file:" << param->get_xml_file() << endl;
	}
	else if( !(param->get_compiler().compare(GCC_COMPILER)) ){
		P = new Parser_gcc();
		P->parseFile( param->get_txt_file());
		cout << "output file:" << param->get_xml_file() << endl;
	}
	else{
		cout << "Sorry, mistake has occured. Please try again." << endl;
//		param ->print_cmd_line();
		help( v1.getImputMethod());
	}
	return 0;
}

