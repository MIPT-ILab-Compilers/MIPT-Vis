/**
 * File: GUI/GuiTextEdit.h - Implementational header for TextEdit of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui/QTextEdit>
#include "gui_impl.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

/**
 * Class GuiTextEdit
 */
class GuiTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    GuiTextEdit();
protected:
	void mousePressEvent( QMouseEvent * mouseEvent);
signals:
	void nodeToBeCentreOn( int nodeNumber);
};

#endif
