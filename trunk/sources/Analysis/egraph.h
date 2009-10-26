/**
 * File: egraph.h - EGraph class definition, part of
 * Analysis library in MiptVis tool.
 * Copyright (C) 2009  AnalysisTeam
 */
#ifndef EGRAPH_H
#define EGRAPH_H

class EGraph: public Graph
{
	ENode* entrynode;
	ENode* exitnode;
	bool is_dom_tree_computed;
	bool is_pdom_tree_computed;
	bool is_cycle_tree_computed;
	bool is_graph_connected;

public:
	EGraph();

    ~EGraph();

    void init();
    
    ENode* getEntry()
    {
        return entrynode;
    }

    void makeGraphSingleEntry();
    
    void makeGraphSingleExit();

    ENode* getExit()
    {
        return exitnode;
    }

    bool isGraphConnected();
    
    ENode* createNode()
    {
        return new ENode( this, incNodeId());
    }

    EEdge* newEdge( ENode * pred, ENode * succ)
    {
        return static_cast< EEdge *>( Graph::newEdge( ( Node*) pred, ( Node*) succ));
    }

    EEdge* firstEdge()
    {
        return static_cast< EEdge *>( Graph::firstEdge());
    }

    ENode* newNode()
    {    
        return static_cast< ENode *>( Graph::newNode());
    }

    ENode* firstNode()
    {
        return static_cast< ENode *>( Graph::firstNode());
    }


};

#endif
