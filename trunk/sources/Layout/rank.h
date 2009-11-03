/**
* File: rank.h - Implementation of rank structure and nodes ordering within it.
* Copyright (C) 2009  MIPTVIS
*/

#ifndef RANK_H
#define RANK_H

#include <QtCore/QVector.h>
#include "layout_iface.h"

/**
* Implementation of layer with nodes of the same rank
* and ordering methods on it
*/

int posCmp( const void*, const void*);
int valCmp( const void*, const void*);

class AdjRank
{
private:

	/** List of nodes within one layer */
	List<NodeAux> adj_rank;

	/** Median value for node v from connected nodes in rank r */
	int medianValue( NodeAux* v);

	/** Barycenter function for node v from connected nodes in rank r */
	int baryValue( NodeAux* v);

	/** Transpose ordering algorithm */
	void transpose();
public:

	inline int rang();

	void debugPrint();

	/** Set initial X coordinates */
	void setInitX();

	/** Ordering nodes
	* Uses median/barycenter and transpose methods maxIter times
	*/
	void doOrder( AdjRank* comp_rank);

	/** Add node to layer */
	void addNode( NodeAux* node)
	{
		adj_rank.addItem( node);
	}

	/** Sorting nodes depends on ordering DFS numeration */
	void sortByNum()
	{
		adj_rank.sort( &posCmp);
	}

	/** Sorting nodes depends on median/barycenter value */
	void sortByVal()
	{
		adj_rank.sort( &valCmp);
	}
};

/**
* Implementation of multiple layers in graph ranking.
*/

class Rank
{
private:
	/** Corresponding graph */
	GraphAux* graph;

	/** List of rank layers */
	QVector<AdjRank>* rank;

	/** Maximum iterations in ordering algorithm */
	static const int maxIter = 24;

	/** Deliver node's ranking from corresponding numeration in auxiliary graph */
	void deliverRank();

	/**
	* Initial ordering of adjacent nodes
	*/
	void initOrder();

public:

	void debugPrint();

	/** Do ordering for graph */
	void doOrderAll();

	/** Default constructor */
	Rank()
	{
	};

	/** Constructor with ranking delivering */
	Rank( GraphAux* ga)
	{
		graph = ga;
		rank = new QVector<AdjRank>( graph->maxRank() + 1);
		deliverRank();
	};

	~Rank()
	{
		delete rank;
	}
};
#endif