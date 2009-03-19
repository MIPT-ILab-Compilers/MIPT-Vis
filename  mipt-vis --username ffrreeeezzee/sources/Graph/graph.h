/**
 * File: graph.h - Graph class definition/implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <libxml/xmlreader.h>

/**
 * Represents properties of a graph
 */
class GraphProperties
{
protected:
	int default_node_size;
	char * name_priv;
	int max_node_id;    
public:
    GraphProperties()
    {
        default_node_size = 10;
	    name_priv = NULL;
	    max_node_id = 0; 
    }
    /**
     * Data retrieving routines
     */
    inline int defaultNodeSize() const
    {
        return default_node_size;
    }
    inline char * name() const
    {
        return name_priv;
    }
	inline int maxNodeId() const
    {
        return max_node_id;  
    }
    /**
     * Data saving routines
     */
    inline void setDefaultNodeSize( int sz)
    {
        default_node_size = sz;
    }
    inline void setName( char *nm)
    {
        name_priv = nm;
    }
	inline void setMaxNodeId( int max_id)
    {
        max_node_id = max_id;  
    }
};

/**
 * Graph class decribes a graph.
 *  Like classical definition G = ( N, E) where N is set of nodes n and E is set of edges e = {n_i, n_j}
 */
class Graph: public MarkerManager, public NumManager, public GraphProperties
{
    /* List of nodes and its iterator */
    NodeListItem* nodes;
    NodeListItem* n_it;
    unsigned long int node_num;
    
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphNum node_next_id;

    /* List of edges and its iterator */
    EdgeListItem* edges;
    EdgeListItem* e_it;
    unsigned long int edge_num;
    
    /** Id of next edge. Incremented each time you create an edge,
     *  needed for edges to have unique id. In DEBUG mode edge id is not reused.
     */
    GraphNum edge_next_id;

	/** Initialization */
	void Init();
protected:
    /** Increment node_next_id and return previous one */
    inline GraphNum IncNodeId()
    {
        return node_next_id++;
    }
    /** Increment edge_next_id and return previous one */
    inline GraphNum IncEdgeId()
    {
        return edge_next_id++;
    }
public:
    /** Constructor */
    Graph();

	/**
	 *  Constructor by XML file
	 */
	Graph( char * filename);

	/**
	 *  Reading form XML
	 */
	void InitNodesFromXmlDoc( xmlNode * a_node);
	void InitEdgesFromXmlDoc( xmlNode * a_node, vector<Node *> nodes);
	void InitFromXMLDoc( xmlNode * a_node);
	void ReadFromXML(const char *filename);
    
    /** Allocation of memory for Edge */
    void * CreateEdge( Node * pred, Node * succ);
    /** Allocation of memory for Node */
    void * CreateNode();

    /** Create new node in graph */
    Node * NewNode();

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    Edge * NewEdge( Node * pred, Node * succ);
    
    /**
     * Remove node from node list of graph
     */
    inline void DeleteNode( NodeListItem* it)
    {
        GraphAssert( IsNotNullP( it));
        if( nodes == it)
        {
           nodes = it->GetNext();
        }
        if( n_it == it)
        {
            n_it = it->GetNext();
        }
        it->Detach();
    }

    /**
     * Remove edge from edge list of graph
     */
    inline void DeleteEdge( EdgeListItem* it)
    {
        GraphAssert( IsNotNullP( it));
        if( edges == it)
        {
            edges = it->GetNext();
        }
        if( e_it == it)
        {
            e_it = it->GetNext();
        }
        it->Detach();
    }

    /**
     * Return node quantity
     */
    inline size_t GetNodeCount() const
    {
        return node_num;
    }

    /**
     * Return edge quantity
     */
    inline size_t GetEdgeCount() const
    {
        return edge_num;
    }
    /*** 
     * Iteration through edges implementation
     *
     * Initialize iterator with first edge and return this edge
     */
    inline Edge* GetFirstEdge() 
    {
        e_it = edges;
        return e_it->GetData();
    }
    /**
     * Advance iterator to next edge and return this edge. If end reached return NULL
     */
    inline Edge* GetNextEdge()
    {
        e_it = e_it->GetNext();
        return (e_it != NULL)? e_it->GetData() : NULL;
    }
    /**
     * return true if end of edge list is reached
     */
    inline bool EndOfEdges()
    {
        return e_it == NULL;
    }
    /*** 
     * Iteration through nodes implementation
     *
     * Initialize iterator with first node and return this node
     */
    inline Node* GetFirstNode()
    {
        n_it = nodes;
        return n_it->GetData();
    }
    /** 
     * Advance iterator to next node and return this node. If end reached return NULL
     */
    inline Node* GetNextNode()
    {
        n_it = n_it->GetNext();
        return ( n_it != NULL)? n_it->GetData() : NULL;
    }
    
    /**
     * return true if end of edge list is reached
     */
    inline bool EndOfNodes()
    {
        return n_it == NULL;
    }

    /**
     * Print graph to stdout in DOT format
     */
    void DebugPrint();

    /**
     * Obtain list of nodes in depth-first search order
     */
    NodeListItem* DFS( Numeration n); 


};

#endif
