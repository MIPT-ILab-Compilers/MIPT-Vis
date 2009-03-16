/**
 * File: graph.cpp - Graph class implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"


/**
 *  Initialize
 */
void Graph::Init()
{
    node_next_id = 0;
    edge_next_id = 0;
    node_num = 0;
    edge_num = 0;
	max_node_id = 0;
    nodes = NULL;
    edges = NULL;
    n_it = NULL;
    e_it = NULL;
}

/**
 * Constructor.
 */
Graph::Graph()
{
	Init();
}

/**
 * Creation node in graph
 */
Node * 
Graph::NewNode()
{
    /**
     * Check that we have available node id 
     */
    GraphAssert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Node *node_p = new Node( this, node_next_id++);
    NodeListItem* it = node_p->GetGraphIt();
    it->Attach( nodes);
    nodes = it;
    node_num++;
    return node_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
Edge * 
Graph::NewEdge( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    GraphAssert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = new Edge( this, edge_next_id++, pred, succ);
    EdgeListItem* it = edge_p->GetGraphIt();
    it->Attach( edges);
    edges = it;
    edge_num++;
    return edge_p;
}

/**
 * Print graph to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
void 
Graph::DebugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = GetFirstNode(); !EndOfNodes(); n = GetNextNode())
    {
        n->DebugPrint();
    }
    /** Print edges */
    for (  e = GetFirstEdge(); !EndOfEdges(); e = GetNextEdge())
    {
        e->DebugPrint();
    }
    out( "}");
}

/**
  * Recursive implementation of depth-first search visit routine
  */
static inline NodeListItem* DfsVisitRec( Node* node, NodeListItem* item, Marker m)
{
    NodeListItem* new_item = new NodeListItem( item, LIST_DIR_RDEFAULT, node);
    Edge *e;
    
    /** Mark node to prevent search from visiting it again */
    node->Mark( m);
    node->DebugPrint();// Print node ID. TODO: Remove this when numeration implementation finished.

    /** Visit Succs */
    for ( e = node->GetFirstSucc(); !node->EndOfSuccs(); e = node->GetNextSucc())
    {
        Node* succ = e->GetSucc();
        if ( !succ->IsMarked( m))
        {
            new_item = DfsVisitRec( succ, new_item, m);
        }
    }
    return new_item;
}

/**
 * Implementation of depth-first search. Starts from nodes without predecessors.
 */
NodeListItem* Graph::DFS()
{
    Node *n;
    GraphNum num = 0;
    NodeListItem *item = NULL;
    Marker m = NewMarker();

    /** Visit nodes */
    for (  n = GetFirstNode(); !EndOfNodes(); n = GetNextNode())
    {
        if( IsNullP( n->GetFirstPred()))
        {
            GraphAssert( !n->IsMarked( m));
            item = DfsVisitRec( n, item, m);
        }
    }   
    
    FreeMarker( m);

    return item;
}

/**
 *  Init graph nodes
 */
void
Graph::InitNodesFromXmlDoc( xmlNode * a_node)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("node")))
		{
			Node *node = NewNode();
			xmlAttr * props;

			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("id")))
				{
					node->id = strtoul( ( const char *)( props->children->content), NULL, 0);
					if ( node->id > max_node_id)
						max_node_id = node->id;
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("color")))
				{
					node->color = ( char *)( props->children->content);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("x")))
				{
					node->x = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("y")))
				{
					node->y = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("width")))
				{
					node->width = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("height")))
				{
					node->height = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("label")))
				{
					node->label = ( char *)( props->children->content);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("shape")))
				{
					node->shape = ( char *)( props->children->content);
				}
			}
		}
	}
}
/**
 *  Init edge points
 */
static void
InitEdgePointsFromXMLDoc( Edge * edge, xmlNode * a_node)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("point")))
		{
			xmlAttr * props;
			int n = -1;
			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("n")))
				{
					n = strtoul( ( const char *)( props->children->content), NULL, 0);
				}
			}

			if ( n == -1) continue;
			edge->points[n] = new EdgePoint;

			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("x")))
				{
					edge->points[n]->x = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("y")))
				{
					edge->points[n]->y = strtoul( ( const char *)( props->children->content), NULL, 0);
				}
			}
		}
	}

}

/**
 *  Init graph edges
 */
void
Graph::InitEdgesFromXmlDoc( xmlNode * a_node, vector<Node *> nodes)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("edge")))
		{
			Edge *edge;
			xmlAttr * props;
			int from = -1, to = -1;

			/** Find 'from' and 'to' */
			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("from")))
				{
					from = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("to")))
				{
					to = strtoul( ( const char *)( props->children->content), NULL, 0);
				}
			}
			if ( from == -1 || to == -1) continue;
			edge = NewEdge( nodes[from], nodes[to]);
			/** Parse other properties */
			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("id")))
				{
					edge->id = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("prob")))
				{
					edge->prob = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("thickness")))
				{
					edge->thickness = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("points_num")))
				{
					edge->points.insert( edge->points.end(), strtoul( ( const char *)( props->children->content), NULL, 0) + 1, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("color")))
				{
					edge->color = ( char *)( props->children->content);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("style")))
				{
					edge->style = ( char *)( props->children->content);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("label")))
				{
					edge->label = ( char *)( props->children->content);
				}
			}
			InitEdgePointsFromXMLDoc( edge, cur_node->children);
		}
	}
}

/**
 * Initializes graph form xmlDoc root node
 */
void
Graph::InitFromXMLDoc( xmlNode * a_node)
{
    xmlNode *cur_node = NULL;
	vector<Node *> nodes;
	bool is_found = false;

    for (cur_node = a_node; cur_node && !is_found; cur_node = cur_node->next)
	{
		if( cur_node->type == XML_ELEMENT_NODE
			&& xmlStrEqual( cur_node->name, xmlCharStrdup("graph")))
		{
			xmlAttr * props;
			Node * node;

			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("default_node_size")))
				{
					default_node_size = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("name")))
				{
					name = ( char *)( props->children->content);
				}
			}

			InitNodesFromXmlDoc( cur_node->children);

			nodes.insert(nodes.end(), ( max_node_id + 1), (Node *)NULL);
			for ( node = GetFirstNode(); !EndOfNodes(); node = GetNextNode())
				    nodes[ node->id] = node;

			InitEdgesFromXmlDoc( cur_node->children, nodes);

			is_found = true;
		}
    }
	nodes.clear();
}

/**
 *  Build graph from xml file.
 */
void
Graph::ReadFromXML(const char *filename)
{
    xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

     /** This initializes the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* Parse the file and get the DOM */
    doc = xmlReadFile(filename, NULL, 0);

    if (doc == NULL)
        printf("error: could not parse file %s\n", filename);

    /* Get the root element node */
    root_element = xmlDocGetRootElement(doc);

	InitFromXMLDoc( root_element);

    /* Free the document */
    xmlFreeDoc(doc);
    /* Cleanup function for the XML library */
    xmlCleanupParser();

}

/**
 *  Constructor by XML filename
 */
Graph::Graph( char * filename)
{
	Init();
	ReadFromXML( filename);
}
