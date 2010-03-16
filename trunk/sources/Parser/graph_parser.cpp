/**
 * File: gcc_parser.cpp - Implementation of Graph class for Parser project
 * Copyright (C) 2009 MIPT-VIS
 */

#include "graph_parser.h"

void ParserGraph::writeToXML( const char *filename)
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


	xmlTextWriterWriteString( writer, BAD_CAST "\t");
	xmlTextWriterStartElement( writer, BAD_CAST "style");
	xmlTextWriterWriteAttribute( writer, BAD_CAST "brush", BAD_CAST "Green");
	xmlTextWriterEndElement( writer);
    xmlTextWriterWriteString( writer, BAD_CAST "\n");
	xmlTextWriterEndElement( writer);


	xmlTextWriterEndDocument( writer);

	xmlFreeTextWriter( writer);
};


void ParserNode::writeByXMLWriter( xmlTextWriterPtr writer)
{
	Node::writeByXMLWriter( writer);

	if ( getColor())
		xmlTextWriterWriteAttribute( writer, BAD_CAST "color", BAD_CAST getColor());
	if ( getText()) 
		xmlTextWriterWriteAttribute( writer, BAD_CAST "textPriv", BAD_CAST getText());
}