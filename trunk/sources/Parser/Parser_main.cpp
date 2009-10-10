// Parser_cmd.cpp : Defines the entry point for the console application.

#include "Parser.h"
#include "Parameters.h"
#include "Functions.h"
#include "Str_to_vector.h"



int main(int argc, char* argv[])
{
	Parser* P;
	vector<string> v;
	Str_to_vector v1;
	bool imput_method;

// Adding parameters to vector 'v' from cmd_line
	imput_method = 0;
	for( int i = 0; i < argc; i++){
		if(i != 0){
			string s(argv[i]);
			v.push_back(s);
		}else;
	}
/*
//Adding parameters to vector 'v' from the string
	imput_method = 1;
	string s = "1.txt 2.xml -gcc";
	v1.parse(s);
	cout << s << endl;
	v = v1.getv();
*/

// Defining the object 'param' that could carry vector v, sorted parameters of cmd_line and specific functions
	Parameters* param = new Parameters(v);

// Sorting and checking parameters
	if( !arrange_parameters(param) )
		help(imput_method);

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
		help(imput_method);
	}
	return 0;
}

