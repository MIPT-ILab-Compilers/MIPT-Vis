/**
 * File: GUI/GuiTextEdit.cpp - Implementation of TextEdit in MiptVis
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#include "gui_textedit.h"

/**
 * Constructor of GuiTextEdit class
 */
GuiTextEdit::GuiTextEdit()
{
}

/**
 * mouse press event
 */
void GuiTextEdit::mousePressEvent( QMouseEvent * mouseEvent)
{
	QPlainTextEdit::mousePressEvent(mouseEvent);
}
