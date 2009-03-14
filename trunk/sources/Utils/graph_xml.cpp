/**
 * File: graph_xml.c -  work with XML holding graph for MiptVis
 * Copyright (C) 2009  Lev Rumyantsev
 */
#include "graph_xml.h"
#include "../Graph/graph_iface.h"
#include <vector>

/**
 *  Init graph nodes
 */
static void
InitGraphNodesFromXmlDoc( Graph * graph, xmlNode * a_node)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("node")))
		{
			Node *node = graph->NewNode();
			xmlAttr * props;

			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("id")))
				{
					node->id = strtoul( ( const char *)( props->children->content), NULL, 0);
					if ( node->id > graph->max_node_id)
						graph->max_node_id = node->id;
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
static void
InitGraphEdgesFromXmlDoc( Graph * graph, xmlNode * a_node, vector<Node *> nodes)
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
			edge = graph->NewEdge( nodes[from], nodes[to]);
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
 * Make graph form xmlDoc root node
 */
static Graph *
MakeGraphFromXMLDoc( xmlNode * a_node)
{
	Graph *graph = NULL;
    xmlNode *cur_node = NULL;
	vector<Node *> nodes;
	bool is_found = false;

    for (cur_node = a_node; cur_node && !is_found; cur_node = cur_node->next)
	{
		if( cur_node->type == XML_ELEMENT_NODE
			&& xmlStrEqual( cur_node->name, xmlCharStrdup("graph")))
		{
			xmlAttr * props;
            graph = new Graph;
			Node * node;

			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("default_node_size")))
				{
					graph->default_node_size = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("name")))
				{
					graph->name = ( char *)( props->children->content);
				}
			}

			InitGraphNodesFromXmlDoc( graph, cur_node->children);

			nodes.insert(nodes.end(), graph->max_node_id + 1, 0);
			for ( node = graph->GetFirstNode();
				  !graph->EndOfNodes();
				  node = graph->GetNextNode())
				    nodes[ node->id] = node;

			InitGraphEdgesFromXmlDoc( graph, cur_node->children, nodes);

			is_found = true;
		}
    }
	nodes.clear();

	return graph;
}

/**
 *  Build graph from xml file.
 */
Graph *
ReadGraphFromXML(const char *filename)
{
    xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	Graph * graph = NULL;

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

	graph = MakeGraphFromXMLDoc( root_element);

    /* Free the document */
    xmlFreeDoc(doc);
    /* Cleanup function for the XML library */
    xmlCleanupParser();

	return graph;
}

