/**
 * File: GUI/gui.cpp - Implementation of GUI in MiptVis
 * Copyright (C) 2009  Lev Rumyantsev, Ivan Mashintsev
 */
#include "gui_impl.h"

/** 
 * Test scene with rectangles and text
 */
bool showScene( int argc, char * argv[], GuiGraph * graph, QApplication * app)
{
    QMainWindow mw;

    if(!graph || !app)
        return false;
    graph->setSceneRect( QRectF( 0, 0, 5000, 5000));

    QGraphicsView * view = new QGraphicsView( graph);
    view->setCacheMode( QGraphicsView::CacheBackground );
    view->setViewportUpdateMode( QGraphicsView::FullViewportUpdate);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setOptimizationFlags( QGraphicsView::DontSavePainterState | QGraphicsView::DontClipPainter);
    if(graph->getNodeItem())
        view->centerOn( graph->getNodeItem());
    view->setWindowTitle( "test_graph2.xml");

	QHBoxLayout *layout = new QHBoxLayout;

	layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    mw.setCentralWidget(widget);

    mw.show();

    app->exec();

	delete view;
	delete layout;
	delete widget;
	
    return true;
}