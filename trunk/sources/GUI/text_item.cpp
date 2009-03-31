/**
 * File: GUI/text_item.cpp - Implementation of text item in MiptVis
 * Copyright (C) 2009  Ivan Mashintsev
 */
#include "gui_impl.h"


/**
 * Constructor for class Text. Create element of the class
 */
Text::Text( QGraphicsItem * parent):QGraphicsTextItem( parent)
{
    setPlainText( "Hello World");
    setTextWidth ( 100); //Set width of text string
    setTextInteractionFlags ( Qt::TextEditable); // Set text can edit
}

/**
 * When forus in the text of the node function run
 */
void Text::focusInEvent( QFocusEvent * event)
{
    parentItem()->setSelected( true); // Selected node which is associated with this text
}

/**
 * When forus out the text of the node function run
 */
void Text::focusOutEvent ( QFocusEvent * event)
{
    parentItem()->setSelected( false); // Unselected node which is associated with this text
}
