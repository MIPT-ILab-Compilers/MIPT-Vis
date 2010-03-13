#include "StyleSheet.h"
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
			Style *st = 0;
			xmlAttr * props = 0;
			QString stname = "default";
			QColor brcol = "black";
			QColor pcol = "magnetta";

			/** Find 'from' and 'to' */
			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("name")))
				{
					stname = ( const char *)( props->children->content);
				}
				else if ( xmlStrEqual( props->name, xmlCharStrdup("brush")))
				{
					brcol = QColor (( const char *)( props->children->content));
				}
				else if ( xmlStrEqual( props->name, xmlCharStrdup("pen")))
				{
					pcol = QColor (( const char *)( props->children->content));
				}
			}
			addStyle (new Style (stname, QPen (pcol), QBrush (brcol)));
		}
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

	sts.push_back (new Style (name, QPen(Qt::black), QBrush()));//if it unique, memorise it
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