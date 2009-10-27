/**
 * File: gcc_parser.cpp - Implementation of parser for GCC compiler
 * Copyright (C) 2009 MIPT-VIS
 */

#include "parser.h"
#include "QtCore/QString"
#include "QtCore/QRegExp"

const QString patt_bb = "^(BBLOCK\\s)(\\d+).*";
const QString patt_predstart = "^preds:.*";
const QString patt_succend = "^succs:.*";
const QString patt_edges = "(\\d+)([^\\d]*)";
const QString patt_context = "^context:.*";


int  Icc_parser::bbNum(std::string &in)
{
	QRegExp regexp;
	QString str(in.c_str());

	regexp.setPattern( patt_bb);
	if ( regexp.indexIn( str) != -1)
		return regexp.cap( 2).toInt();
	else
		return -1;
}

bool Icc_parser::getPred( string &in, BBlock &bb)
{
	QRegExp regex;
	QString str( in.c_str());
	int pos;
	
	regex.setPattern( patt_predstart);
	pos = regex.indexIn( str);
	if( pos == -1)
		return false;

	size_t cnt = strlen( "preds: ");
	regex.setPattern( patt_edges);
	while ( 1 )
	{
		pos = regex.indexIn( str.mid( (int)cnt));
		if ( pos == -1)
		{
			bb.setPredsInstd();
			break;
		}
		bb.addPred( regex.cap( 1).toInt());
		cnt += regex.matchedLength();
	}
	return true;
}


bool Icc_parser::getSucc( string &in, BBlock &bb)
{
	QRegExp regex;
	QString str( in.c_str());
	int pos;
	
	regex.setPattern( patt_succend);
	pos = regex.indexIn( str);
	if( pos == -1)
		return false;

	size_t cnt = strlen( "succs: ");
	regex.setPattern( patt_edges);
	while ( 1 )
	{
		pos = regex.indexIn( str.mid( (int)cnt));
		if ( pos == -1)
		{
			bb.setSuccsInstd();
			break;
		}
		bb.addSucc( regex.cap( 1).toInt());
		cnt += regex.matchedLength();
	}
	return true;
}

bool Icc_parser::isContext(std::string &in)
{
	QRegExp regexp;
	QString str(in.c_str());

	regexp.setPattern( patt_context);
	return regexp.indexIn( str) != -1;
}

bool Icc_parser::parseFromStream(std::istream &is)
{
	int strnum = 0;
	bool isContextFound = false;
	BBlock * tbb;

	dump_info.addFunction();

	while ( !is.eof())
	{
		string current;
		int tmp;

		getline( is, current);
		strnum++;

		if ( ( tmp = bbNum( current)) != -1)
		{
			isContextFound = false;
			tbb = dump_info.addBBlock( tmp, strnum);

			while ( !getPred( current, *tbb))
			{
				getline( is, current);
				strnum++;
			}

			while ( !getSucc( current, *tbb))
			{
				getline( is, current);
				strnum++;
			}
			
			continue;
		}
		
		if ( isContextFound)
			tbb->addText( current, strnum);

		isContextFound |= isContext( current);
	}

	return true;
}
