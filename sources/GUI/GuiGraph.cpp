/**
 * File: GUI/GuiGraph.cpp - Implementation of GuiGraph in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"

/**
 * Create element after double click mouse.
 */
void GuiGraph::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if( mouseEvent->button() != Qt::LeftButton)
    {
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
        return;  
    }
    if( selectedItems().isEmpty())
    {
        number++;
        GuiNode * node = newNode();
        node->setPos( mouseEvent->scenePos());
        node->setColor( "green");
        node->setLabel( "NODE");
        node->setShape( "rectangle");
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
        guiNodes << node;
    }
    QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( QObject * parent):myMode( insertRect), Graph(), QGraphicsScene( parent)
{
    number = 0;
	line = NULL;
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( char * filename, QObject * parent):myMode( insertRect), Graph(), QGraphicsScene( parent)
{
    number = 0;
	line = NULL;
    readFromXML( filename);
}
/**
 * Create line after right button clicked
 */
void GuiGraph::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if( mouseEvent->button() & Qt::RightButton)
    {
        myMode = insertLine;
        line = new QGraphicsLineItem( QLineF( mouseEvent->scenePos(), mouseEvent->scenePos()));
        line->setPen( QPen( Qt::black));
        addItem( line);
    }
    update();
    QGraphicsScene::mousePressEvent( mouseEvent);
}

/**
 * Move line when move mouse.
 */
void GuiGraph::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if ( myMode == insertLine && line != 0)
    {
        QLineF newLine( line->line().p1(), mouseEvent->scenePos());
        line->setLine( newLine);
    } 
    else if ( myMode == moveItem)
    {
        QGraphicsScene::mouseMoveEvent( mouseEvent);
    }
}

/**
 * Insert line on the scene after release the mouse.
 */
void GuiGraph::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if ( line != 0 && myMode == insertLine)
    {
        QList<QGraphicsItem *> startItems = items( line->line().p1());
        if ( startItems.count() && startItems.first() == line)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items( line->line().p2());
        if ( endItems.count() && endItems.first() == line)
        {
            endItems.removeFirst();
        }
        removeItem( line);
        delete line;
        if ( startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first() != endItems.first() && 
            startItems.first()->type() == GuiNode::Type &&
            endItems.first()->type() == GuiNode::Type)
        {
            GuiNode * startItem = qgraphicsitem_cast< GuiNode *> ( startItems.first());
            GuiNode * endItem = qgraphicsitem_cast< GuiNode *> ( endItems.first());
            GuiEdge * lin = newEdge( startItem, endItem);
            guiEdges << lin;
        }
    }
    line = NULL;
    myMode = moveItem;
    QGraphicsScene::mouseReleaseEvent( mouseEvent);
}

/**
 * Creation node in graph
 */
GuiNode * GuiGraph::newNode()
{
    GuiNode * node = ( GuiNode *) createNode();
    node->setZValue( 1);  
    addItem( node);
    return node;
}

/**
 * Allocation memory for edge
 */
void * GuiGraph::createEdge( GuiNode * pred, GuiNode * succ)
{
    return new GuiEdge( this, incEdgeId(), pred, succ);
}

/**
 * Allocation memory for node
 */
void * GuiGraph::createNode()
{
    int num = incNodeId();
    QString text =  QString( "Node %1").arg( num);
    return new GuiNode( &text, this, num);
}
/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
GuiEdge * GuiGraph::newEdge( GuiNode * pred, GuiNode * succ)
{
    GuiEdge * e =  ( GuiEdge *) createEdge( pred, succ);
    addItem( e);
    e->initPoint( pred, succ);
    e->initPoints( 1);
    e->setStyle( "Solid");
    pred->addEdge( e);
    succ->addEdge( e);
    e->setZValue( -1000.0);
    e->updatePosition();
    return e;
}

void GuiGraph::initPos()
{
    for( Node * nd = firstNode(); nd != NULL; nd = nextNode())
    {
        GuiNode * gn = newNode();
        gn->setPos( (qreal)nd->x(), (qreal)nd->y());
        
    }
}
/**
 *  Init graph nodes
 */
void GuiGraph::readNodesFromXmlDoc( xmlNode * a_node)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("node")))
		{
			GuiNode * node = newNode();
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
            node->setPos( node->Node::x(), node->Node::y()); 
            guiNodes << node;
		}
	}
}

/**
 *  Init edge points
 */
static void readEdgePointsFromXMLDoc( GuiEdge * edge, xmlNode * a_node)
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
			edge->setPoint( new EdgePoint, n);
			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("x")))
				{
					edge->point( n)->x = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("y")))
				{
					edge->point( n)->y = strtoul( ( const char *)( props->children->content), NULL, 0);
				}
                
			}
		}
	}

}

/**
 *  Init graph edges
 */
void GuiGraph::readEdgesFromXmlDoc( xmlNode * a_node, QVector< GuiNode *> nodes)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("edge")))
		{
			GuiEdge *edge;
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
			readEdgePointsFromXMLDoc( edge, cur_node->children);
            guiEdges << edge;
		}
	}
}

/**
 * Initializes graph form xmlDoc root node
 */
void GuiGraph::readFromXMLDoc( xmlNode * a_node)
{
    xmlNode * cur_node = NULL;
	QVector< GuiNode *> nodes;
	bool is_found = false;

    for (cur_node = a_node; cur_node && !is_found; cur_node = cur_node->next)
	{
		if( cur_node->type == XML_ELEMENT_NODE
			&& xmlStrEqual( cur_node->name, xmlCharStrdup("graph")))
		{
			xmlAttr * props;
			GuiNode * node;

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
			nodes.insert(nodes.end(), ( max_node_id + 1), (GuiNode *)NULL);
			for ( int i = 0; i<guiNodes.count(); i++)
            {
                node = guiNodes.at( i);
				nodes[ node->userId()] = node;
            }
            readEdgesFromXmlDoc( cur_node->children, nodes);
			is_found = true;
		}
    }
	nodes.clear();
}

/**
 *  Build graph from xml file.
 */
void GuiGraph::readFromXML(const char *filename)
{
    xmlDoc * doc = NULL;
	xmlNode * root_element = NULL;

     /** This initializes the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* Parse the file and get the DOM */
    doc = xmlReadFile( filename, NULL, 0);

    if ( doc == NULL)
    {
        printf( "error: could not parse file %s\n", filename);
    }
    /* get the root element node */
    root_element = xmlDocGetRootElement( doc);

	readFromXMLDoc( root_element);

	this->xml_doc = doc;

	/* Cleanup function for the XML library */
    xmlCleanupParser();
}

/**
 *  Write nodes
 */
void GuiGraph::writeNodesByXMLWriter( xmlTextWriterPtr writer)
{
	GuiNode * node;
    for ( int i = 0; i<guiNodes.count(); i++)
    {
        node = guiNodes.at( i);
        node->writeByXMLWriter( writer);
    }
}

/**
 *  Write edges
 */
void GuiGraph::writeEdgesByXMLWriter( xmlTextWriterPtr writer)
{
	GuiEdge * edge;
    for ( int i = 0; i < guiEdges.count(); i++)
    {
        edge = guiEdges.at( i);
        edge->writeByXMLWriter( writer);
    }
}

/**
 *  Write graph to xml file
 */
void GuiGraph::writeToXML( const char *filename)
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
