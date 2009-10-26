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
public:
    ENode( EGraph *graph, int _id);

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
    ENode* nextNode()
    {
        return static_cast< ENode *>( Node::nextNode());
    }
};

#endif
