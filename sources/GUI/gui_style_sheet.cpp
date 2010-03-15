#include "gui_style_sheet.h"
#include "libXml/tree.h"

/*
 * constructor
 */
Style::Style (QString name_, QPen pen_, QBrush brush_)
	:name_priv (name_), pen(pen_), brush(brush_)
{
}

/*
 * apply style to painter
 */
void Style::applayTo (QPainter * painter) const
{
	painter->setPen (pen);
	painter->setBrush (brush);
}

/*
 * load style from xml
 */
void Style::loadFromXmlNode (xmlNode * a_node)
{
	xmlAttr * props = 0;
	QColor brcol = "black";
	QColor pcol = "magnetta";
	qreal pwidth = 2;

	/** Find 'from' and 'to' */
	for( props = a_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup("name")))
		{
			name_priv = ( const char *)( props->children->content);
		}
		else if ( xmlStrEqual( props->name, xmlCharStrdup("brush")))
		{
			brcol = QColor (( const char *)( props->children->content));
		}
		else if ( xmlStrEqual( props->name, xmlCharStrdup("penColor")))
		{
			pcol = QColor (( const char *)( props->children->content));
		}
		else if ( xmlStrEqual( props->name, xmlCharStrdup("penWidth")))
		{
			pwidth = atof (( const char *)( props->children->content));
		}
	}
	pen = QPen (pcol, pwidth);
	brush = QBrush (brcol);
}

/*
 * Write style to XML document
 */
void Style::writeByXMLWriter( xmlTextWriterPtr writer)
{
	xmlTextWriterWriteAttribute( writer, BAD_CAST "name", BAD_CAST name_priv.toAscii().data());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "brush", BAD_CAST brush.color().name().toAscii().data());
	xmlTextWriterWriteAttribute( writer, BAD_CAST "penColor", BAD_CAST pen.color().name().toAscii().data());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "penWidth", "%d", BAD_CAST pen.width());
}

/*
 * constructor
 */
StyleSheet::StyleSheet()
{
}

/*
 * destrcutor
 */
StyleSheet::~StyleSheet()
{
	for (QVector <Style*>::iterator i = sts.begin(); i != sts.end(); ++i)
		delete *i;
}

/*
 * load style table from xml
 */
void StyleSheet::loadFromXmlNode (xmlNode * a_node)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("style")))
		{
			Style *st = new Style ("default", QPen(), QBrush(Qt::white));
			st->loadFromXmlNode (cur_node);
			addStyle (st);
		}
	}
}

/*
 * Write styles to XML document
 */
void StyleSheet::writeByXMLWriter( xmlTextWriterPtr writer)
{
	for (int i = 0; i < sts.size(); ++i)
	{
		xmlTextWriterWriteString( writer, BAD_CAST "\t");
		xmlTextWriterStartElement( writer, BAD_CAST "style");

		sts[i]->writeByXMLWriter (writer);

		xmlTextWriterEndElement( writer);
	    xmlTextWriterWriteString( writer, BAD_CAST "\n");
	}
}

/*
 * Get style id by it's name
 */
StId StyleSheet::getId (const QString& name)
{
	//for (QVector <Style*>::const_iterator i = sts.begin(); i != sts.end(); ++i)
	//	if ((**i).name() == name)
	//		return i;
	for (int i = 0; i < sts.size(); ++i)
		if (sts[i]->name() == name)
			return i;

	sts.push_back (new Style (name, QPen(Qt::black), QBrush (Qt::white)));//if it unique, memorise it
	return sts.count() - 1;
}

/*
 * Apply style by id
 */
void StyleSheet::applayStyle (StId id, QPainter * painter)
{
	sts[id]->applayTo (painter);
}

/*
 * Apply style by name
 */
void StyleSheet::applayStyle (const QString& name, QPainter * painter)
{
	sts[getId (name)]->applayTo (painter);
}

/*
 * Get style name
 */
const QString& StyleSheet::getStName (StId id)
{
	return sts[id]->name();
}
/*
 * Add style
 */
StId StyleSheet::addStyle (Style* st)
{
	for (int i = 0; i < sts.size(); ++i)
		if (sts[i]->name() == st->name()) //check if style with same name already exists
		{
			delete sts[i];//change old style be new
			sts[i] = st;
			return i;
		}
	sts.push_back (st);//if it unique, memorise it
	return sts.size() - 1;
}