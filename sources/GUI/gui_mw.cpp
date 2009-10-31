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

    currentFile = "";

    graph = new GuiGraph();
    graph->setSceneRect( QRectF( 0, 0, 5000, 5000));
    connect(graph, SIGNAL(isClicked()), this, SLOT(textHandle()));

    view = new GuiView(graph);
    view->setScene(graph);
    
    if(graph->getNodeItem())
      view->centerOn( graph->getNodeItem());
    
    setCurrentFile(currentFile);

    nodeTextEdit = new QPlainTextEdit;
    nodeTextEdit->clear();
    nodeTextEdit->setReadOnly(true);
    
    confirmButton = new QPushButton(tr("Save &Text"));
    confirmButton->setEnabled(false);
    confirmButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(saveTextToNode()));

    textLayout = new QVBoxLayout;
    textLayout->addWidget(nodeTextEdit);
    textLayout->addWidget(confirmButton);

    groupBox = new QGroupBox(tr("Text of selected node"));
    groupBox->setLayout(textLayout);
    groupBox->setMinimumWidth(150);
    groupBox->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

    splitter = new QSplitter;
    splitter->addWidget(view);
    splitter->addWidget(groupBox);
 
    layout = new QHBoxLayout;
    layout->addWidget(splitter);

    widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
}

/**
 * Load a file
 */
void MainWindow::load()
{
    currentFile = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("XML (*.xml);;All files(*.*)")); 
    if (currentFile.isEmpty()) return;
    QByteArray curFile = currentFile.toAscii();
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

    setCurrentFile(currentFile);
}

/**
 * Save current file
 */
void MainWindow::save()
{
    currentFile = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr("XML (*.xml);;All files(*.*)")); 
    if (currentFile.isEmpty()) return;
    QByteArray curFile = currentFile.toAscii();
    char *file;
    file = (char*) calloc(curFile.size(),sizeof(char));
    if (file==NULL) return;
    int i;
    for (i=0;i<=curFile.size();++i) file[i]=curFile[i];
    graph->writeToXML( file);
    setCurrentFile(currentFile);
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
 * centreOnNode
 */
void MainWindow::centreOnNode()
{
    int nodeId = QInputDialog::getInteger(this,"Center On Node","Enter Node Number",0,-1000,1000,1,0,0);
    GuiNode * node;
    for ( node = ( GuiNode *)graph->firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if (node->id() == nodeId)
      {
		  view->centerOn(node);
          break;
	  }
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

    centreOnNodeAct = new QAction(tr("&Centre On Node"), this);
    centreOnNodeAct->setStatusTip(tr("Centre On Node..."));
    connect(centreOnNodeAct, SIGNAL(triggered()), this, SLOT(centreOnNode()));
}

/**
 * Creat menus
 */
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(centreOnNodeAct);

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
        /** Boris: This is error, type cast without checking. This pointer can point to GuiEdge as well */
		//node = (GuiNode*) list[0]; // Next line fails if list[0] actually points to edge item		
		/** Instead let's use qgraphicsitem_cast */
		if ( qgraphicsitem_cast< GuiNode*>( list[ 0]))
		{
			GuiNode *node = qgraphicsitem_cast< GuiNode*>( list[ 0]);
			nodeTextEdit->setPlainText( node->myText);
			nodeTextEdit->setReadOnly(false);
			confirmButton->setEnabled(true);
		}
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
        node = (GuiNode*) list[0];//!!!Here probably nessary qgraphicsitem_cast too
        QString str;
        str = nodeTextEdit->toPlainText();
        node->setMyText(str);
        nodeTextEdit->clear();
        nodeTextEdit->setReadOnly(true);
        confirmButton->setEnabled(false);
    }
}

/**
 * setCurrentFile
 */
void MainWindow::setCurrentFile(const QString & fileName)
{
    QString shownName;
    if (fileName.isEmpty()) shownName = "untitled.xml";
    else shownName = strippedName(fileName);
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("MIPT-Vis")));
}

/**
 * setCurrentFile
 */
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
} 
