/**
 * File: enode.h - ENode class definition, part of
 * Analysis library in MiptVis tool.
 * Copyright (C) 2009  AnalysisTeam
 */
#ifndef ENODE_H
#define ENODE_H

class ENode: public Node
{
    friend class EGraph;
    ENode* idom;
    ENode* pdom;

	/** Set of nodes for which current node is a semidominator */
	vector< ENode*> bucket;
	/* Number of semidominator of current node */
	int semi;
	/** Node which is parent of current node in the spanning tree generated by DFS */
	ENode* parent;
	ENode* ancestor;
	ENode* label;

public:
    ENode( EGraph *graph, int _id);
    
	/**
	 * Gives the value of immediate dominator and postdominator of current node.
	 */
	void setidom( ENode* idm, ENode* node)
    {
         node->idom = idm;
    }
    void setpdom( ENode* pdm, ENode* node)
    {
         node->pdom = pdm;
    } 
    
    ENode* getidom()
    {
         return idom;
    }
    ENode* getpdom()
    {
         return pdom;
    }
	/**
	 * Casts to bring functions of class to the functions of advanced class
	 */
	int getSemi()
	{
		return semi;
	}

	void setSemi( int n)
	{
		semi = n;
	}

    ENode* nextNode()
    {
        return static_cast< ENode *>( Node::nextNode());
    }
};

#endif
