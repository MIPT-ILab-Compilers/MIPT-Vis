/**
* File: rank.h - Implementation of rank structure and nodes ordering within it.
* Copyright (C) 2009  MIPTVIS
*/

#ifndef RANK_H
#define RANK_H

#include <QtCore/QLinkedList.h>
#include "layout_iface.h"

/**
* Implementation of layer with nodes of the same rank
* and ordering methods on it
*/

class AdjRank
{
private:

	/** List of nodes within one layer */
	QLinkedList<NodeAux*> adjRank;

	/** Maximum iterations in ordering algorithm */
	static const int maxIter = 24;

	/**
	* Initial ordering of adjacent nodes
	* Sorting nodes depends on some numeration
	*/
	void initOrder()
	{};
	
	/** Median value for node v from connected nodes in rank r */
	int medianValue( NodeAux* v, AdjRank* r)
	{};

	/** Median ordering algorithm */
	void medianOrder()
	{};

	/** Transpose ordering algorithm */
	void transpose()
	{};
public:
	/** Ordering nodes
	* Uses median and transpose methods maxIter times
	*/
	void doOrder()
	{
		initOrder();
		for( int i = 0; i < maxIter; i++)
		{
			medianOrder();
			transpose();
		}
	};
};

/**
* Implementation of multiple layers in graph ranking.
*/

class Rank
{
private:
	/** List of rank layers */
	QLinkedList <AdjRank> rank;
public:
	/** Deliver node's ranking from corresponding numeration in auxiliary graph */
	void deliverRank( GraphAux* graph)
	{
	};

	/** Default constructor */
	Rank()
	{
	};

	/** Constructor with ranking delivering */
	Rank( GraphAux* graph)
	{
		deliverRank( graph);
	};
};
#endif