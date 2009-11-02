/**
* File: rank.cpp - Implementation of rank structure and nodes ordering within it.
* Copyright (C) 2009  MIPTVIS
*/

#include "layout_iface.h"

/** Compare two nodes by their DFS numeration */
int posCmp(  const void *x, const void *y)
{
	int res;
	NodeAux* nodeX = ( NodeAux*)x;
	NodeAux* nodeY = ( NodeAux*)y;
	Numeration dfs_num = nodeX->getGraph()->getDfsNumeration();
	int posX = nodeX->number( dfs_num);
	int posY = nodeY->number( dfs_num);
	//debug
	out( "numeration:");
	nodeX->debugPrint();
	out( "pos: %d", posX);
	nodeY->debugPrint();
	out( "pos: %d", posY);
	//
	if ( posX < posY) res =  -1;
	if ( posX > posY) res = 1;
	if ( posX == posY) res = 0;
	return res;
}

/** Print all nodes off layer with some info */

void AdjRank::debugPrint()
{
	for( ListItem< NodeAux>* iter=adj_rank.head(); iter!=NULL; iter = iter->next())
	{
		iter->data()->debugPrint();
	}
}

/** Deliver node's ranking from corresponding numeration in auxiliary graph */
void Rank::deliverRank()
{
	for( NodeAux* iter = graph->firstNode(); iter != NULL; iter = iter->nextNode())
	{
		(*rank)[ iter->rang()].addNode( iter);
	}
	debugPrint();
};

/**
* Initial ordering of adjacent nodes
*/
void Rank::initOrder()
{
	Numeration dfs_num = graph->newNum();
	graph->DFS( dfs_num);
	graph->setDfsNumeration( dfs_num);
	for( NodeAux* iter = graph->firstNode(); iter!=NULL; iter = iter->nextNode())
	{
		iter->debugPrint();
		out("dfs_numeration %d", iter->number( dfs_num));
	}
	for( QVector< AdjRank>::iterator iter = rank->begin(); iter != rank->end(); iter++)
	{
		iter->sortByNum();
	}
	graph->freeNum( dfs_num);
}

/** Do ordering for graph */
void Rank::doOrderAll()
{
	initOrder();
	for( QVector< AdjRank>::iterator iter = rank->begin(); iter != rank->end(); iter++)
	{
		iter->doOrder();
	}
}


/** Print all layers in rank */
void Rank::debugPrint()
{
	int i = 0;
	for( QVector<AdjRank>::iterator iter = rank->begin(); iter != rank->end(); iter++)
	{
		out( "Layer %d", i);
		iter->debugPrint();
		i++;
	}
}
