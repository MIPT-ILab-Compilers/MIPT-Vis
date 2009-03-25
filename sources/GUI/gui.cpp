/**
 * File: GUI/gui.cpp - Implementation of GUI in MiptVis
 * Copyright (C) 2009  Lev Rumyantsev
 */
#include "gui_impl.h"
#include <QtGui/QApplication>
#include <QtGui/QLabel>

bool showGraph( int argc, char * argv[], Graph * graph)
{
 	QApplication app( argc, argv);
	QLabel *label = new QLabel("Qt works!");
	label->show();
	app.exec();
	return true;
}