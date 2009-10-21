/**
 * File: GUI/GuiMW.cpp - Implementation of MainWindow in MiptVis
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
    createStatusBar();

    fileName = "";

    graph = new GuiGraph();
    graph->setSceneRect( QRectF( 0, 0, 5000, 5000));

    view = new GuiView(graph);
    view->setScene(graph);
    
    if(graph->getNodeItem())
      view->centerOn( graph->getNodeItem());
    
    view->setWindowTitle( fileName);

    label = new QLabel;
    label ->setText("");

    connect(graph, SIGNAL(aNodeIsClicked(const QString &)), label, SLOT(setText(const QString &)));

    textLayout = new QVBoxLayout;
    textLayout->addWidget(label);

    groupBox = new QGroupBox(tr("Content of selected node"));
    groupBox->setLayout(textLayout);

    layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addWidget(groupBox);

    widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
}

/**
 * Load a file
 */
void MainWindow::load()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("XML (*.xml);;All files(*.*)")); 
    if (fileName.isEmpty()) return;
    QByteArray curFile = fileName.toAscii();
    char *file;
    file = (char*) calloc(curFile.size(),sizeof(char));
    if (file==NULL) return;
    int i;
    for (i=0;i<=curFile.size();i++) file[i]=curFile[i];

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (graph!=NULL) delete graph;
	graph = new GuiGraph( file);
    view->setScene(graph);
    connect(graph, SIGNAL(aNodeIsClicked(const QString &)), label, SLOT(setText(const QString &)));

    QApplication::restoreOverrideCursor();

    view->setWindowTitle( fileName);
}

/**
 * Save current file
 */
void MainWindow::save()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr("XML (*.xml);;All files(*.*)")); 
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
 * Help
 */
void MainWindow::help()
{
    QMessageBox::about(this, tr("Mipt-Vis Help"),
      tr("To open a file :  On the File menu, click Open\n\n"\
         "To save the current file :  On the File menu, click Save\n\n"\
         "To zoom in and out :  Use the mouse wheel"));
}

/**
 * About
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Mipt-Vis"),
      tr("  Mipt-Vis is a project that is developed in the Intel-MIPT "
         "students laboratory by a group of students from "
         "Moscow Institute of Physics and Technology( MIPT) "
         "under Intel employees mentoring.\n\n  Mipt-vis is an "
         "open source software product that helps you to "
         "visualize and perform different analyses on "
         "compiler-specific graphs. Also it is educational project "
         "that gives its members more knowledge about compiler science "
         "and work in team.\n\n  There are four components: "
         "GUI( mentor Rumyantsev Lev), Graph placement( mentor Shurygin Boris), "
         "Graph analyses( mentor Zolotukhin Michael) and Parser( mentor Sidorenko Ivan). \n\n"
         "  At first IR from GCC and ICC will be supported."));
}

/**
 * Do Layout
 */
void MainWindow::doLayoutSlot()
{
    graph->doLayout();
}

/**
 * Creat actions
 */
void MainWindow::createActions()
{
    loadAct = new QAction(tr("&Load"), this);
    loadAct->setStatusTip(tr("Load..."));
    connect(loadAct, SIGNAL(triggered()), this, SLOT(load()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setStatusTip(tr("Save..."));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    helpAct = new QAction(tr("Mipt-Vis &Help"), this);
    helpAct->setStatusTip(tr("Help..."));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    aboutAct = new QAction(tr("&About Mipt-Vis"), this);
    aboutAct->setStatusTip(tr("About..."));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    doLayoutAct = new QAction(tr("Do &Layout"), this);
    doLayoutAct->setStatusTip(tr("Do Layout..."));
    connect(doLayoutAct, SIGNAL(triggered()), this, SLOT(doLayoutSlot()));
}

/**
 * Creat menus
 */
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(doLayoutAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
}

/**
 * Creat status bar
 */
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
