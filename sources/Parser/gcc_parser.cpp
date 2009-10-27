/**
 * File: gcc_parser.cpp - Implementation of parser for GCC compiler
 * Copyright (C) 2009 MIPT-VIS
 */

#include "parser.h"
#include "QtCore/QString"
#include "QtCore/QRegExp"

const QString patt_fn = "^(;;\\sFunction\\s)(.+) .*";
const QString patt_defbb = "^(Basic\\sblock\\s)(\\d+).*";
const QString patt_implbb = "^(;;\\sStart\\sof\\sbasic\\sblock\\s)(\\d+).*";
const QString patt_implbbend = "^(;;\\sEnd\\sof\\sbasic\\sblock\\s)(\\d+).*";
const QString patt_predstart = "^(Predecessors:\\s*)ENTRY.*";
const QString patt_succend = "^Successors:\\s*EXIT.*";
const QString patt_edges = "(\\d+)([^\\d]*)";


/** Current status of parser */
struct Status
{
	string in_function;
	int in_bblock_def;
	int in_bblock_impl;
};



string Gcc_parser::fnName(string &in)
{
	QRegExp regexp;
	QString str( in.c_str());

	regexp.setPattern( patt_fn);
	if ( regexp.indexIn( str) != -1)
	{
		return regexp.cap( 2).toAscii().constData();
		/* QString::toStdString() method crashes on my computer */
	}
		else
	return "";
}

int Gcc_parser::defbbNum(string &in)
{
	QRegExp regexp;
	QString str( in.c_str());

	regexp.setPattern( patt_defbb);
	if ( regexp.indexIn( str) != -1)
		return regexp.cap( 2).toInt();
	else
		return -1;
}

int Gcc_parser::implbbNum(string &in)
{
	QRegExp regexp;
	QString str( in.c_str());

	regexp.setPattern( patt_implbb);
	if ( regexp.indexIn( str) != -1)
		return regexp.cap( 2).toInt();
	else
		return -1;
}

int Gcc_parser::implbbEnd(string &in)
{
	QRegExp regexp;
	QString str( in.c_str());

	regexp.setPattern( patt_implbbend);
	if ( regexp.indexIn( str) != -1)
		return regexp.cap( 2).toInt();
	else
		return -1;
}

void Gcc_parser::getPred(string &in, BBlock &bb)
{
	QRegExp regex;
	QString str( in.c_str());
	int pos;

	regex.setPattern( patt_predstart);
	pos = regex.indexIn( str);
	if( pos != -1)
		bb.setPredsInstd();
	else
	{	
		size_t cnt = strlen( "Predecessors:  ");
		regex.setPattern( patt_edges);
		while ( 1 )
		{
			pos = regex.indexIn( str.mid( (int)cnt));
			if ( pos == -1)
				break;
			bb.addPred( regex.cap( 1).toInt());
			cnt += regex.matchedLength();
		}
	}
}

void Gcc_parser::getSucc(string &in, BBlock &bb)
{
	QRegExp regex;
	QString str( in.c_str());
	int pos;

	regex.setPattern( patt_succend);
	pos = regex.indexIn( str);
	if( pos != -1)
		bb.setSuccsInstd();
	else
	{	
		size_t cnt = strlen( "Successors:  ");
		regex.setPattern( patt_edges);
		while ( 1 )
		{
			pos = regex.indexIn( str.mid( (int)cnt));
			if ( pos == -1)
				break;
			bb.addSucc( regex.cap( 1).toInt());
			cnt += regex.matchedLength();
		}
	}
}

/**
 * Generate dump_info from input data
 */
bool Gcc_parser::parseFromStream( istream & is)
{
	int strnum = 0;
	Status status={ "", -1, -1};
	string lastFn;

	while(!is.eof())
	{
		string current, str;
		int tmp;
		BBlock *cbb;   //Current basic block

		getline( is, current);
		strnum++;

		/* Function declaration found */
		if ( !( str = fnName( current)).empty())
		{
			lastFn = str;
			dump_info.addFunction( lastFn);
			continue;
		}

		/* gBBlock definition found */
		if ( ( tmp = defbbNum( current)) != -1)
		{
			cbb = dump_info.addBBlock( tmp, strnum, lastFn);;
			getline ( is, current);
			strnum++;
			getPred( current, *cbb);
			getline( is, current);
			strnum++;
			getSucc( current, *cbb);
			continue;
		}

		/* gBBlock implementatin found */
		if ( (tmp = implbbNum( current)) != -1)
		{
			cbb = dump_info.getBBlock( tmp, lastFn);
			getline( is, current);
			strnum++;
			do 
			{
				cbb->addText( current, strnum);
				getline( is, current);
				strnum++;
			}while ( implbbEnd( current) != tmp);

			continue;
		}
	}

	return  NULL;	
}
