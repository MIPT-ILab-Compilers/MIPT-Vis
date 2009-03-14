/**
 * File: graph_xml.h -  work with XML holding graph for MiptVis
 * Copyright (C) 2009  Lev Rumyantsev
 */
#ifndef GRAPH_XML_IFACE_H
#define GRAPH_XML_IFACE_H

#include <libxml/xmlreader.h>
class Graph;

Graph * ReadGraphFromXML(const char *filename);

#endif