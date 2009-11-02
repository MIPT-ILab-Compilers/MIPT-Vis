/**
 * File: parser_impl.cpp - Implementation of parser basic class and helper-classes
 * Copyright (C) 2009 MIPT-VIS
 */


#include "parser.h"
/**
 * BBlock class methods
 */

BBlock::BBlock( int line) 
{
	def = line;
	instd_preds = instd_succs = false;
	instd_text = false;
	impl = -1;
}

bool BBlock::isFinished()
{
	return instd_preds &&
		   instd_succs;
	     // instd_text :fails on gcc dumps 
}

void BBlock::addText( string & txt, int line)
{
	if ( !text.empty())
		text += '\n';
	text += txt;
	instd_text = true;
	impl = ( impl == -1) ? line : impl;
}


/**
 * Function class methods
 */

BBlock * Function::addBBlock( int number, int line)
{
	if ( lastAdded != -1 && ! getBBlock( lastAdded)->isFinished())
		throw exIncorrectSequence(lastAdded, number);

	BBlocks::iterator it;

	it = blocks.find( number);
	if ( it != blocks.end())
		throw exAlreadyExists( line);
	BBlock * tmp = new BBlock( line);
	blocks.insert( pair< int, BBlock> ( number, *tmp));
	lastAdded = number;
	return &blocks[ number];
}

BBlock * Function::getBBlock( int number)
{
	map < int, BBlock>::iterator it;

	it = blocks.find( number);
	if ( it == blocks.end())
		throw exNotFound( number);
	return &it->second;
}

Graph * Function::getGraph()
{
	Graph * graph = new Graph;
	map< int, Node *> node_list;
	BBlocks::const_iterator i;

	for ( i = getFirstBBlock(); i != getEndBBlock(); i++)
	{
		node_list[ i->first] = graph->newNode();
//		node_list[ i->first]->setLabel( "node");
//		node_list[ i->first]->setHeight( 10);
//		node_list[ i->first]->setWidth( 20);
//		node_list[ i->first]->setX( 0);//Here I'm not know: what do you mean?
//		node_list[ i->first]->setY( 0);
//		node_list[ i->first]->setColor( "red");
//		node_list[ i->first]->setShape( "bar");
	}

	map< int, Node *>::const_iterator node_index;
	for ( node_index = node_list.begin();
		  node_index != node_list.end();
		  node_index++
		)
	{
		Lint::const_iterator to;
		BBlock * bb= getBBlock( node_index->first);

		for ( to = bb->getFirstSucc();
			  to != bb->getEndSucc();
			  to++
			)
		{
			if ( *to != -1)
				graph->newEdge(node_list[ node_index->first], node_list[ *to]);
		}
	}

	return graph;
}


/**
 * Dump info class methods
 */

Function * DumpInfo::addFunction( string & name)
{
	if ( funcs.find( name) != funcs.end())
		throw exAlreadyExists( name);;
	return &funcs[ name];
}

Function * DumpInfo::getFunction( string & name)
{
	if ( funcs.find( name) == funcs.end())
		throw exNotFound( name);
	return &funcs[ name];
}

BBlock * DumpInfo::getBBlock( int num, string &name)
{
	return getFunction( name)->getBBlock( num);
}

BBlock * DumpInfo::addBBlock( int number, int line, string &name)
{
	return getFunction( name)->addBBlock( number, line);
}

Graph * DumpInfo::getGraph( const char *fname)
{
	string tmp = ( fname)? string( fname) : "main";
	return getFunction( tmp)->getGraph();
}

/**
 * Parser class methods
 */

bool Parser::parseFile( string filename)
{
	ifstream is( filename.c_str(), ios::in);
	if ( is.is_open())
		return parseFromStream( is);
	else
		return NULL;
}
