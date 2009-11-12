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

	ENode* findSubgraphEntry(ENode* node, Marker mrk);
	ENode* findSubgraphExit( ENode* node, Marker mrk);
	void visitAllSuccs( ENode *node, Marker m);
	void visitAllPreds( ENode *node, Marker m);
    void markSubgFromNode( ENode* node, Marker m);


public:
	EGraph();

    ~EGraph();

    void init();
    
    ENode* getEntry()
    {
        return entrynode;
    }
    ENode* getExit()
    {
        return exitnode;
    }
    /**
     * makeGraphSingleEntry/Exit - includes supportive additional nodes to the graph
     * to make the entry - a node, from which you can come at any node going through the whole graph,
     * or an exit - a node, at which you can come from any node going through the whole graph.
     */
    void makeGraphSingleEntry();
    
    void makeGraphSingleExit();
    
    bool isGraphConnected();
    
    /**
     * Casts to bring functions of class to the functions of advanced class
     */
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
