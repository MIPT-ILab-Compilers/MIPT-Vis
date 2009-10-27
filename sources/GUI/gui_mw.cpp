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
    connect(graph, SIGNAL(isClicked()), this, SLOT(textHandle()));

    view = new GuiView(graph);
    view->setScene(graph);
    
    if(graph->getNodeItem())
      view->centerOn( graph->getNodeItem());
    
    view->setWindowTitle( fileName);

    nodeTextEdit = new QPlainTextEdit;
    nodeTextEdit->clear();
    nodeTextEdit->setReadOnly(true);
    
    confirmButton = new QPushButton(tr("Save &Text"));
    confirmButton->setEnabled(false);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(saveTextToNode()));

    textLayout = new QVBoxLayout;
    textLayout->addWidget(nodeTextEdit);
    textLayout->addWidget(confirmButton);

    groupBox = new QGroupBox(tr("Content of selected node"));
    groupBox->setLayout(textLayout);

    rightLayout = new QVBoxLayout;
    rightLayout->addWidget(groupBox);

    layout = new QHBoxLayout;
    layout->addWidget(view);
    layout->addLayout(rightLayout);

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
    connect(graph, SIGNAL(isClicked()), this, SLOT(textHandle()));

    nodeTextEdit->clear();
    nodeTextEdit->setReadOnly(true);
    confirmButton->setEnabled(false);

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
    graph->doLayout();//!!! An error occurs when after layout, one calls load
    update();//!!! When the edges' successors are changed, they continue point to old ones !!!
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

/**
 * Text Handle
 */
void MainWindow::textHandle()
{
	QList<QGraphicsItem*> list = graph->selectedItems();
	if (list.size()==1)
	{
        GuiNode *node;
        node = (GuiNode*) list[0];
        QString str;
        str = node->myText;
        nodeTextEdit->setPlainText(str);
        nodeTextEdit->setReadOnly(false);
        confirmButton->setEnabled(true);
	}
	else
	{
        nodeTextEdit->clear();
        nodeTextEdit->setReadOnly(true);
        confirmButton->setEnabled(false);
	}
}

/**
 * saveTextToNode
 */
void MainWindow::saveTextToNode()
{
	QList<QGraphicsItem*> list = graph->selectedItems();
	if (list.size()==1)
	{
        GuiNode *node;
        node = (GuiNode*) list[0];
        QString str;
        str = nodeTextEdit->toPlainText();
        node->setMyText(str);
        nodeTextEdit->clear();
        nodeTextEdit->setReadOnly(true);
        confirmButton->setEnabled(false);
	}
}

