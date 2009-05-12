/**
 * File: graph.cpp - Graph class implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"
#include <deque>
#include <stack>

/**
 *  Initialize
 */
void Graph::init()
{
    node_next_id = 0;
    edge_next_id = 0;
    node_num = 0;
    edge_num = 0;
	nodes = NULL;
    edges = NULL;
    n_it = NULL;
    e_it = NULL;
	xml_doc = NULL;
}

/**
 * Constructor.
 */
Graph::Graph()
{
	init();
}
/**
 * Allocation of memory for Edge
 */
Edge *Graph::createEdge( Node *pred, Node *succ)
{
    return new Edge( this, incEdgeId(), pred, succ);
}
/**
 * Allocation of memory for Node
 */
Node *Graph::createNode()
{
    return new Node( (Graph *)this, incNodeId());
}

/**
 * Creation node in graph
 */
Node * 
Graph::newNode()
{
    /**
     * Check that we have available node id 
     */
    graphassert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Node *node_p = ( Node *) createNode();
    NodeListItem* it = node_p->getGraphIt();
    it->attach( nodes);
    nodes = it;
    node_num++;
    return node_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
Edge * 
Graph::newEdge( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    graphassert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = ( Edge *) createEdge( pred, succ);
    EdgeListItem* it = edge_p->getGraphIt();
    it->attach( edges);
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
Graph::debugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = firstNode(); !endOfNodes(); n = nextNode())
    {
        n->debugPrint();
    }
    /** Print edges */
    for (  e = firstEdge(); !endOfEdges(); e = nextEdge())
    {
        e->debugPrint();
    }
    out( "}");
}

/**
  * Recursive implementation of depth-first search visit routine
  */
static inline NodeListItem*
dfsVisitRec( Node* node,
             NodeListItem* item,
             Marker m,
             Numeration n,
             GraphNum* number)
{
    /** attach the element to the list with already visited elements*/
    NodeListItem* new_item = new NodeListItem( item, LIST_DIR_RDEFAULT, node);
    Edge *e;

    /** mark node to prevent search from visiting it again */
    node->mark( m);
    node->setNumber( n, *number);
    *number = (*number) + 1;

    /** Visit Succs skipping already visited */
    for ( e = node->firstSucc(); !node->endOfSuccs(); e = node->nextSucc())
    {
        Node* succ = e->succ();
        if ( !succ->isMarked( m))
        {
            /** Continue DFS recursively with the successor */
            new_item = dfsVisitRec( succ, new_item, m, n, number);
        }
    }
    return new_item;
}

/**
 * Implementation of depth-first search visit routine with using a stack
 */
static inline NodeListItem*
dfsVisitStack( Node* node,
             NodeListItem* item,
             Marker m,
             Numeration n,
             GraphNum* number)
{
    /** attach the element to the list with already visited elements*/
    NodeListItem* new_item = new NodeListItem( item, LIST_DIR_RDEFAULT, node);
    Edge *e;
	stack<Node *> st;
	st.push( node);
	while ( st.empty())
	{
		/** Execute the node from a stack 
         * If the top of stack isn't the node without predecessors,
	     * then current node is the last successor of its predecessor
	     */
		node = st.top();
		st.pop();
        /** Mark node to prevent search from visiting it again */
        node->mark( m);
        node->setNumber( n, *number);
        *number = (*number) + 1;

        /** Visit Succs skipping already visited */
        for ( e = node->firstSucc(); !node->endOfSuccs(); e = node->nextSucc())
        {
            Node* succ = e->succ();
            if ( !succ->isMarked( m))
            {
                /** Writing Succs in a stack */
				st.push( succ);
                
            }
        }
	}
    return new_item;
}

/**
 * Implementation of breadth-first search. Starts from node without predecessors.
 * Fuction return last node without predessors
 */
NodeListItem* Graph::BFS( Numeration num)
{
	deque<Node*> q;    //queue with two ends
    GraphNum number = 0;
    NodeListItem *item = NULL;
	Node *child;
	Node *n;
	Edge *e;
	/** Create a marker to distinguish visited nodes from unvisited ones */
    Marker m = newMarker();
	
	for (  n = firstNode(); !endOfNodes(); n = nextNode())
    {
        /** 
         * If firstPred is Null, then the node n has no predecessors, =>
         * we should start BFS from this node.
         */

    if( isNullP( n->firstPred()))
        {
            graphassert( !n->isMarked( m));
			item = new NodeListItem( item, LIST_DIR_RDEFAULT, n);
            /** 
             * Start BFS from node n, using marker m and numeration num.
             * item is a list that at the end will contain all nodes in BF order.
             * number is the number of the last visited node.
             */
            q.push_back( n);
	       while ( !q.empty())
	       {
			   n = q.front();
			   q.pop_front();
			   n->setNumber( num,number);
		       number = ( number) + 1;
		       n->mark( m);
		       for ( e = n->firstSucc(); !n->endOfSuccs(); e = n->nextSucc())
		       {
			       child = e->succ();
			       if ( !child->isMarked( m)) continue;
			       q.push_back( child);
			   }
		   }
		}
	}      
	freeMarker( m);
	return item;
}

/**
 * Implementation of depth-first search. Starts from nodes without predecessors.
 */
NodeListItem* Graph::DFS( Numeration num)
{
    Node *n;
    GraphNum number = 0;
    NodeListItem *item = NULL;
    /** Create a marker to distinguish visited nodes from unvisited ones */
    Marker m = newMarker();

    /** 
     * Visit all nodes and call dfsVisitRec for each node that doesn't have predecessors.
     * The other nodes are skipped here, but they will be visited in dfsVisitRec.
     * If the graph is connected, we'll call dfsVisitRec only once - for the start node.
     */
    for (  n = firstNode(); !endOfNodes(); n = nextNode())
    {
        /** 
         * If firstPred is Null, then the node n has no predecessors, =>
         * we should start DFS from this node.
         */
        if( isNullP( n->firstPred()))
        {
            graphassert( !n->isMarked( m));
            /** 
             * Start DFS from node n, using marker m and numeration num.
             * item is a list that at the end will contain all nodes in DF order.
             * number is the number of the last visited node.
             */
            /** 
             * Start DFS using a stack
             */
            item = dfsVisitStack( n, item, m, num, &number);
            /** 
             * Start DFS using recursion
             */
			// item = dfsVisitRec( n, item, m, num, &number);
        }
    }   
    /** 
     * TODO: introduce assert(all nodes have been visited) 
     */

    freeMarker( m);

    return item;
}

/**
 *  Init graph nodes
 */
void
Graph::readNodesFromXmlDoc( xmlNode * a_node)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("node")))
		{
			Node *node = newNode();
			xmlAttr * props;

			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("id")))
				{
					node->setUserId( strtoul( ( const char *)( props->children->content), NULL, 0));
					if (  node->userId() > maxNodeId())
						setMaxNodeId( node->userId());
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("color")))
				{
					node->setColor( ( char *)( props->children->content));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("x")))
				{
					node->setX( strtoul( ( const char *)( props->children->content), NULL, 0));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("y")))
				{
					node->setY( strtoul( ( const char *)( props->children->content), NULL, 0));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("width")))
				{
					node->setWidth( strtoul( ( const char *)( props->children->content), NULL, 0));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("height")))
				{
					node->setHeight( strtoul( ( const char *)( props->children->content), NULL, 0));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("label")))
				{
					node->setLabel( ( char *)( props->children->content));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("shape")))
				{
					node->setShape( ( char *)( props->children->content));
				}
			}
		}
	}
}



/**
 *  Init graph edges
 */
void
Graph::readEdgesFromXmlDoc( xmlNode * a_node, vector<Node *> nodes)
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
			edge = newEdge( nodes[from], nodes[to]);
			/** Parse other properties */
			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("id")))
				{
					edge->setUserId( 
                        strtoul( ( const char *)( props->children->content), NULL, 0) );
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("prob")))
				{
					edge->setProb( 
                        strtoul( ( const char *)( props->children->content), NULL, 0) );
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("thickness")))
				{
					edge->setThickness(
                        strtoul( ( const char *)( props->children->content), NULL, 0) );
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("points_num")))
				{
                    edge->initPoints( 
                        strtoul( ( const char *)( props->children->content), NULL, 0) + 1);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("color")))
				{
					edge->setColor( ( char *)( props->children->content));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("style")))
				{
					edge->setStyle( ( char *)( props->children->content));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("label")))
				{
					edge->setLabel( ( char *)( props->children->content));
				}
			}
			edge->readEdgePointsFromXMLDoc( cur_node->children);
		}
	}
}

/**
 * Initializes graph form xmlDoc root node
 */
void
Graph::readFromXMLDoc( xmlNode * a_node)
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
					setDefaultNodeSize( strtoul( ( const char *)( props->children->content), NULL, 0));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("name")))
				{
					setName( ( char *)( props->children->content));
				}
			}

			readNodesFromXmlDoc( cur_node->children);
			nodes.insert(nodes.end(), ( max_node_id + 1), (Node *)NULL);
			for ( node = firstNode(); !endOfNodes(); node = nextNode())
				    nodes[ node->userId()] = node;

			readEdgesFromXmlDoc( cur_node->children, nodes);
			is_found = true;
		}
    }
	nodes.clear();
}

/**
 *  Build graph from xml file.
 */
void
Graph::readFromXML(const char *filename)
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

    /* get the root element node */
    root_element = xmlDocGetRootElement(doc);

	readFromXMLDoc( root_element);

	this->xml_doc = doc;

	/* Cleanup function for the XML library */
    xmlCleanupParser();
}

/**
 *  Constructor by XML filename
 */
Graph::Graph( char * filename)
{
	init();
	readFromXML( filename);
}

/**
 *  Write nodes
 */
void
Graph::writeNodesByXMLWriter( xmlTextWriterPtr writer)
{
	Node * node;
	for ( node = firstNode(); !endOfNodes(); node = nextNode())
	{
		node->writeByXMLWriter( writer);
	}
}

/**
 *  Write edges
 */
void
Graph::writeEdgesByXMLWriter( xmlTextWriterPtr writer)
{
	Edge * edge;
	for ( edge = firstEdge(); !endOfEdges(); edge = nextEdge())
	{
		edge->writeByXMLWriter( writer);
	}
}

/**
 *  Write graph to xml file
 */
void
Graph::writeToXML( const char *filename)
{
	xmlTextWriterPtr writer;


    /* Create a new XmlWriter for uri, with no compression. */
    writer = xmlNewTextWriterFilename( filename, 0);
    assert( writer != NULL);
    
    xmlTextWriterStartDocument( writer, NULL, NULL, NULL);
	xmlTextWriterStartElement( writer, BAD_CAST "graph");
	xmlTextWriterWriteString( writer, BAD_CAST "\n");

	xmlTextWriterWriteAttribute( writer, BAD_CAST "name", BAD_CAST name());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "default_node_size",
		                               "%d", defaultNodeSize());

	writeNodesByXMLWriter( writer);
    writeEdgesByXMLWriter( writer);

	xmlTextWriterEndElement( writer);

	xmlTextWriterEndDocument( writer);
}
/**
 * Implementation for numerations cleanup
 */
void
Graph::clearNumerationsInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); !endOfNodes(); n = nextNode())
    {
        clearUnusedNumerations( static_cast<Numbered *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); !endOfEdges(); e = nextEdge())
    {
        clearUnusedNumerations( static_cast<Numbered *>(n));
    }
}

/**
 * Implementation for markers cleanup
 */
void
Graph::clearMarkersInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); !endOfNodes(); n = nextNode())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); !endOfEdges(); e = nextEdge())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
}

/**
 *  Destructor of graph class
 */
Graph::~Graph()
{
    /* Free the document */
    if ( xml_doc != NULL)xmlFreeDoc( xml_doc);

}