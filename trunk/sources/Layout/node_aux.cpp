#include "layout_iface.h"

NodeAux::NodeAux( GraphAux *graph_p, int _id):
    Node( static_cast<Graph *>( graph_p), _id)
{
		setReal (true);
}
//-----------------------------------------------------------------------------
NodePropertiesAux::NodePropertiesAux()
{
    // Default position is first in first layer
    rang_priv = 0;
    pos_priv = 1;

	x_priv = 0;
	y_priv = 0;
	width_priv = 0;
	height_priv = 0;
	is_real = true;
}
//-----------------------------------------------------------------------------
void NodeAux::debugPrint()
{
	Node::debugPrint();
	out (" rang: %i; pos: %i; real:%i", rang_priv, pos_priv, (int)real());
}
//-----------------------------------------------------------------------------

/**
 *  Write to xml
 */
void NodeAux::writeByXMLWriter( xmlTextWriterPtr writer)
{
	Node::writeByXMLWriter (writer);
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "x", "%d", x());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "y", "%d", y());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "width", "%d", width());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "height", "%d", height());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "real", "%d", (int)real());
}
//-----------------------------------------------------------------------------
/**
 *  Read from xml
 */
void NodeAux::readByXML (xmlNode * cur_node)
{
	Node::readByXML (cur_node);

	for (xmlAttr* props = cur_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup("x")))
		{
			setX( strtoul( ( const char *)( props->children->content), NULL, 0));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup("y")))
		{
			setY( strtoul( ( const char *)( props->children->content), NULL, 0));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup("real")))
		{
			setReal ((bool)strtoul( ( const char *)( props->children->content), NULL, 0));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup("width")))
		{
			setWidth( strtoul( ( const char *)( props->children->content), NULL, 0));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup("height")))
		{
			setHeight( strtoul( ( const char *)( props->children->content), NULL, 0));
		}
	}
}
//-----------------------------------------------------------------------------
