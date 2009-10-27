/**
 * File: parser.h - Parser class definition.
 * Copyright (C) 2009  MIPTVIS
 */
#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <map>
#include <string>
#include <list>
#include <algorithm>

#include "../Graph/graph_iface.h"


#define ICC_COMPILER "-icc"
#define GCC_COMPILER "-gcc"
#define UNDEFINED "\n"


/**
 * Implementation of Basic Block
 */
typedef list < int> Lint;

class BBlock
{
	Lint preds, succs;             //Predcessors & successors
	bool instd_preds, instd_succs; //Preds & succs installed?
	int def, impl;                 //String names of definition and implementation, first-start, second-end
	string text;                   //Implementaton of basic block
	bool instd_text;               //Implementation installed?

public:
	BBlock( int line);
	BBlock() {}
	bool isFinished();
	void setPredsInstd() { instd_preds = true; } //Set pedcessors status to "Installed"
	void setSuccsInstd() { instd_succs = true; } //Same for successors
	void addPred( int pred) { preds.push_back( pred); setPredsInstd(); }
	void addSucc( int succ) { succs.push_back( succ); setSuccsInstd(); }
	void addText( string & txt, int line);
	Lint::const_iterator getFirstSucc() { return succs.begin(); }
	Lint::const_iterator getEndSucc() { return succs.end(); }
};


/**
 * Implementation of function
 */
typedef map < int, BBlock> BBlocks;

class Function
{
	BBlocks blocks;
	int lastAdded;

	BBlocks::const_iterator getFirstBBlock() { return blocks.begin(); }
	BBlocks::const_iterator getEndBBlock() { return blocks.end(); }

public:
	Function() { lastAdded = -1; }
	BBlock * addBBlock( int number, int line);
	BBlock * getBBlock( int number);
	Graph * getGraph();
};

/**
 * Imnplementaton of DumpInfo
 */
typedef map< string, Function> Functions;

class DumpInfo
{
	Functions funcs;

public:
	Function * addFunction( string & name = string("main"));
	Function * getFunction( string & name = string("main"));
	BBlock * addBBlock( int number, int line, string & name = string("main"));
	BBlock * getBBlock( int num, string & name = string("main"));
	Graph * getGraph( const char *fname = NULL);
};


/**
 * Exception types for errors handling
 */
class exSomething
{
public:
	virtual void wtf( ostream &os)
	{
		os << "General error!" << endl;
	}
};

class exNotFound: public exSomething
{
	int  number;
	string fnName;

public:
	exNotFound( int num) { number = num; }
	exNotFound( string & name) { fnName = name; number = -1; }

	void wtf( ostream &os)
	{
		if ( number == -1)
			os << "Function " << fnName;
		else 
			os << "Basic block " << number;

		os << " not found!" << endl;
	}

};

class exAlreadyExists: public exSomething
{
	int number;
	string fnName;

public:
	exAlreadyExists( int num) { number = num; }
	exAlreadyExists( string & name) { fnName = name; number = -1; }

	void wtf( ostream &os)
	{
		if ( number == -1)
			os << "Function " << fnName;
		else 
			os << "Basic block " << number;

		os << " already exists!" << endl;
	}

};

class exIncorrectSequence: public exSomething
{
	int current, next;

public:
	exIncorrectSequence( int notClosed, int added)
	{
		current = notClosed;
		next = added;
	}

	void wtf( ostream &os)
	{
		os << "Basic block " << current << " not finished, but basic block "\
			<< next << " opened!" << endl;
	}
};

/**
 * Main class of Parser tool
 */
class Parser
{
public:
	Parser(){}
	virtual ~Parser(){}
    bool parseFile( string filename);
	virtual Graph * getGraph( const char *fname = NULL)=0;

protected:
    virtual bool parseFromStream( istream & is) = 0;
};


class Gcc_parser: public Parser
{
private:
	DumpInfo dump_info;

	string fnName( string &in);
	int defbbNum( string &in);
	int implbbNum( string &in);
	int implbbEnd( string &in);
	void getPred( string &in, BBlock &bb);
	void getSucc( string &in, BBlock &bb);
	
public:
	Graph * getGraph( const char *fname = NULL) { return dump_info.getGraph( fname); }
	bool parseFromStream( istream & is);
	list < string> & getFunctionList();
	Gcc_parser() { }
	~Gcc_parser() {	}
};


class Icc_parser: public Parser
{
private:
	DumpInfo dump_info;

	int bbNum( string &in);
	bool getPred( string &in, BBlock &bb);
	bool getSucc( string &in, BBlock &bb);
	bool isContext( string &in);
	
public:
	Graph * getGraph( const char *fname = NULL) { return dump_info.getGraph( fname); }
	bool parseFromStream( istream & is);
	Icc_parser() { }
	~Icc_parser() {	}
};


#endif //PARSER_H