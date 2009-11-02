/**
 * File: node.cpp - Node class implementation, part of Graph library, 
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"

NodeProperties::NodeProperties()
{
	user_id = 0;
    color_priv = 0;
	x_priv = 0;
	y_priv = 0;
	width_priv = 0;
	height_priv = 0;
	label_priv = 0;
	shape_priv = 0;
	text_priv = 0;
	is_real = true;
}
/** 
 * Destructor. Corrects list of nodes in corresponding graph and deletes adjacent edges
 */
Node::~Node()
{
	out ("deleting node# "); debugPrint();
	out ("\n\n\n");

    Edge *edge;
    
    /** delete incidient edges */
    for ( edge = firstSucc(); isNotNullP( edge);)
    {
        Edge* next = edge->nextSucc();
        edge->detachFromNode( GRAPH_DIR_DOWN);// Edge is detached from succ node
        delete edge;
        edge = next;
    }
    for ( edge = firstPred(); isNotNullP( edge);)
    {
        Edge* next = edge->nextPred();
        edge->detachFromNode( GRAPH_DIR_UP);// Edge is detached from pred node
        delete edge;
        edge = next;
    }
    /** delete myself from graph */
    graph->detachNode( this);
}

/**
 * Add an edge to this node in specified direction
 */
void
Node::addEdgeInDir( Edge *edge, GraphDir dir)
{
    graphassert( isNotNullP( edge));
    EdgeListItem *it = edge->getNodeIt( revDir( dir));
    it->attach( edges[ dir]);
    edges[ dir] = it;
}

/**
 * delete edge pointed by iterator in specidied direction
 */
void
Node::deleteEdgeInDir( GraphDir dir, EdgeListItem* it)
{
    graphassert( isNotNullP( it));
    if( edges[ dir] == it)
    {
        edges[ dir] = it->next();
    }
    if( e_it[ dir] == it)
    {
        e_it[ dir] = it->next();
    }
    it->detach();
}

/**
 * Print node in Dot format to stdout
 */
void
Node::debugPrint()
{
    out("%u;", id());
}

/**
 *  Write to xml
 */
void
Node::writeByXMLWriter( xmlTextWriterPtr writer)
{
	xmlTextWriterWriteString( writer, BAD_CAST "\t");
	xmlTextWriterStartElement( writer, BAD_CAST "node");

	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%d", id());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "x", "%d", x());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "y", "%d", y());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "width", "%d", width());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "height", "%d", height());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "real", "%d", (int)real());
	if (label()) xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST label());
	if (color()) xmlTextWriterWriteAttribute( writer, BAD_CAST "color", BAD_CAST color());
	if (shape()) xmlTextWriterWriteAttribute( writer, BAD_CAST "shape", BAD_CAST shape());
	if (textPriv()) xmlTextWriterWriteAttribute( writer, BAD_CAST "textPriv", BAD_CAST textPriv());
	
	xmlTextWriterEndElement( writer);
    xmlTextWriterWriteString( writer, BAD_CAST "\n");
}