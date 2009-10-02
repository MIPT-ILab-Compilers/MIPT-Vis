#include "Parser.h"
#include "Parameters.h"


void help (void){
	cout << "Usage: \"parser.exe file_name.txt param1 param2(optional)\"" << endl;
	cout << "Param1: " << ICC_COMPILER << " or " << GCC_COMPILER << endl;
	cout << "	" << ICC_COMPILER << " -Use ICC compiler" << endl;
	cout << "	" << GCC_COMPILER << " -Use GCC compiler" << endl;
	cout << "Param2(Optional): parsed_file_name.xml" << endl;
	cout << "	Parsed file will be saved as 'parsed_file_name.xml'" << endl;
	cout << "	On default 'file_name.xml' will be created." << endl;
}

int check_len ( int len ){
	if ( len < 3 || len > 4 ) return 0;										//Mistake - illegal number of parameters
	else return 1;															
}

bool if_compiler_defined_before ( Parameters* p )
{
	if ( !(p ->get_compiler() ).compare(UNDEFINED) ) return false;			//Compiler is not defined yet
	else return true;														//Compiler is already defined
}

bool if_this_is_compiler ( string s )
{
	if (!(s.compare(ICC_COMPILER)) || !(s.compare(GCC_COMPILER)) ) return true;
	else return false;
}

bool if_txt_file_defined_before ( Parameters* p )
{
	if ( !(p ->get_txt_file() ).compare(UNDEFINED) ) return false;			//txt file is not defined yet
	else return true;														//txt file is already defined
}

bool if_this_is_txt_file ( string s )
{
	string s1;
	if( s.size()>=5){
		s1 = s;
		s1.erase(s1.begin(), s1.end() - sizeof(".txt") + 1 );
		if( !s1.compare(".txt") ) return true;								//this is txt file
	}
	else return false;														//this is not txt file
}

bool if_xml_file_defined_before ( Parameters* p )
{
	if ( !(p ->get_xml_file() ).compare(UNDEFINED) ) return false;			//xml file is not defined yet
	else return true;														//xml file is already defined
}

bool if_this_is_xml_file ( string s )
{
	string s1;
	if( s.size()>=5){
		s1 = s;
		s1.erase(s1.begin(), s1.end() - sizeof(".xml") + 1 );
		if( !s1.compare(".xml") ) return true;								//this is xml file
	}
	else return false;														//this is not xml file
}
void make_xml_file_name_from_txt_file_name( Parameters* p )
{
	string s = p ->get_txt_file();
	string s1(s);
	s1.erase(s1.end() - sizeof(".txt") + 1, s1.end());
	s1.append(".xml");
	p ->set_xml_file(s1);
}

int arrange_parameters ( Parameters* param ){
	string l;
	if ( !check_len ( param->get_len() ) ){
		cerr << "The number of parameters doesn't match." << endl << endl;
		return 0;
	}
	else{
		while ( param->get_len() > 1 ){	
			l = (param ->get_next_param());
			if ( if_compiler_defined_before ( param ) && if_this_is_compiler ( l ) ){
				cerr << "Mistake - too many compilers. Please choose -icc or -gcc." << endl;
				return 0;
			}
			else if ( if_txt_file_defined_before ( param ) && if_this_is_txt_file ( l ) ){
				cerr << "Mistake - too many txt files." << endl;
				return 0;
			}
			else if ( if_xml_file_defined_before ( param ) && if_this_is_xml_file ( l ) ){
				cerr << "Mistake - too many xml files." << endl;
				return 0;
			}
			else if ( if_this_is_compiler ( l ) ) param ->set_compiler( l );
			else if ( if_this_is_txt_file ( l ) ) param ->set_txt_file( l );
			else if ( if_this_is_xml_file ( l ) ) param ->set_xml_file( l );
			else{
				cerr << "Mistake - unexpected filename or compiler." << endl;
				return 0;
			}

		}
		if( !if_compiler_defined_before ( param ) ){
			cerr << "Mistake - undefined compiler." << endl;
			return 0;
		}
		else if( !if_txt_file_defined_before ( param ) ){
			cerr << "Mistake - undefined txt_file." << endl;
			return 0;
		}
		else if( !if_xml_file_defined_before ( param ) )
			make_xml_file_name_from_txt_file_name( param );
		return 1;
	}
}