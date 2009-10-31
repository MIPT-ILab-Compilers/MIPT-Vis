/**
 * File: edge.cpp - Edge class implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"

/**
 * Edge destructor.
 * delete edge from graph's list of edges
 */

Edge::~Edge()
{
	out ("deleting edge: "); debugPrint();
	out ("\n\n\n");

	detachFromNode (GRAPH_DIR_UP);
	detachFromNode (GRAPH_DIR_DOWN);
	graph->detachEdge( this);
}

/**
 * Print edge in DOT format to stdout
 */

void
Edge::debugPrint()
{
    /**
     * Check that edge is printable
     * TODO: Implements graph states and in 'in process' state print node as '?'
     *       Examples of such prints: 4->? ?->3 ?->?
     */
	int predid = -1;
	int succid = -1;
	if (isNotNullP( pred())) predid = pred()->id();
	if (isNotNullP( succ())) succid = succ()->id();
//    graphassert( isNotNullP( pred()));
//    graphassert( isNotNullP( succ()));

    out("%u->%u;", predid, succid);
}

/**
 * Low level correction of node's edge list in corresponding direction
 */

void
Edge::detachFromNode( GraphDir dir)
{
    Node *n = getNode( dir);
    if (n) n->deleteEdgeInDir( revDir( dir), &n_it[ dir]);
	nodes[dir] = 0;
	
}

/**
 *  Read edge points
 */
void
Edge::readEdgePointsFromXMLDoc( xmlNode * a_node)
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

			Node* my_point = graph->insertNodeOnEdge (this);
			my_point->setReal (false);

			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("x")))
				{
					my_point->setX (strtoul( ( const char *)( props->children->content), NULL, 0));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("y")))
				{
					my_point->setY (strtoul( ( const char *)( props->children->content), NULL, 0));
				}
			}
			
		}
	}

}

/**
 * Write edge points by xml writer
 */
void
Edge::writePointsByXMLWriter( xmlTextWriterPtr writer)
{
}

/**
 * Write edge by xml writer
 */
void
Edge::writeByXMLWriter( xmlTextWriterPtr writer)
{
	xmlTextWriterWriteString( writer, BAD_CAST "\t");
	xmlTextWriterStartElement( writer, BAD_CAST "edge");

	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%d", id());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "from", "%d", pred()->id());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "to", "%d", succ()->id());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST label());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "prob", "%d", prob());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "thickness", "%d", thickness());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "color", BAD_CAST color());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "style", BAD_CAST style());

    xmlTextWriterWriteString( writer, BAD_CAST "\n");
	writePointsByXMLWriter( writer);

	xmlTextWriterWriteString( writer, BAD_CAST "\t");
	xmlTextWriterEndElement( writer);
    xmlTextWriterWriteString( writer, BAD_CAST "\n");
}