#pragma once
#ifndef STYLE_SHEET_H
#define STYLE_SHEET_H
#include <QtGui/QPainter>
#include <Qt/QVector.h>
#include <Qt/QString.h>

#include "libXml/tree.h"
//struct xmlNode;

class Style
{
	QString name_priv;
	QPen pen;
	QBrush brush;

public:
	Style (QString name, QPen pen, QBrush brush);
	virtual void applayTo (QPainter * painter) const;

	inline void rename (QString nname)
	{
		name_priv = nname;
	}

	inline const QString& name() const
	{
		return name_priv;
	}
};

class EdgeStyle
{
};

typedef int/*QList <Style*>::const_iterator*/ StId;

class StyleSheet
{
	QVector <Style*> sts;

public:
	StyleSheet();
	~StyleSheet();

	void loadFromXmlNode (xmlNode * a_node);

	StId getId (const QString& name);
	void applayStyle (StId id, QPainter * painter);
	void applayStyle (const QString& name, QPainter * painter);
	StId addStyle (Style* st);
};

#endif // STYLE_SHEET_H