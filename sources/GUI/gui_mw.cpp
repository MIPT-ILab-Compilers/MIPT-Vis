/**
 * File: GUI/GuiMW.cpp - Implementation of GuiEdge in MiptVis
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#include <QtGui/QtGui>
#include "gui_mw.h"

/**
 * Constructor of MainWindow class
 */
MainWindow::MainWindow()
{
	createActions();
    createMenus();

	fileName = "";

	graph = new GuiGraph();
	graph->setSceneRect( QRectF( 0, 0, 5000, 5000));

    view = new QGraphicsView( graph);
    view->setCacheMode( QGraphicsView::CacheBackground );
    view->setViewportUpdateMode( QGraphicsView::FullViewportUpdate);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setOptimizationFlags( QGraphicsView::DontSavePainterState | QGraphicsView::DontClipPainter);
    if(graph->getNodeItem())
        view->centerOn( graph->getNodeItem());
    view->setWindowTitle( fileName);

	layout = new QHBoxLayout;
	layout->addWidget(view);

    widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
}

/**
 * Load a file
 */
void MainWindow::load()
{
    fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty()) return;
	QByteArray curFile = fileName.toAscii();
	char *file;
	file = (char*) calloc(curFile.size(),sizeof(char));
	if (file==NULL) return;
	int i;
	for (i=0;i<=curFile.size();i++) file[i]=curFile[i];

	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	delete graph;
	graph = new GuiGraph( file);
	view->setScene(graph);

    QApplication::restoreOverrideCursor();

	view->setWindowTitle( fileName);
}

/**
 * Save current file
 */
void MainWindow::save()
{
    fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty()) return;
	QByteArray curFile = fileName.toAscii();
	char *file;
	file = (char*) calloc(curFile.size(),sizeof(char));
	if (file==NULL) return;
	int i;
	for (i=0;i<=curFile.size();++i) file[i]=curFile[i];
	graph->writeToXML( file);
	view->setWindowTitle( fileName);
}

/**
 * Creat actions of menu File
 */
void MainWindow::createActions()
{
    loadAct = new QAction(tr("&Load"), this);
    loadAct->setStatusTip(tr("Load..."));
    connect(loadAct, SIGNAL(triggered()), this, SLOT(load()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setStatusTip(tr("Save..."));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
}

/**
 * Creat menu File of MainWindow
 */
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);
}
