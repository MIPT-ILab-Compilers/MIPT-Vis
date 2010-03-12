/**
 * File: GUI/GuiMW.cpp - Implementation of MainWindow in MiptVis
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#include <QtGui/QtGui>
#include "gui_mw.h"
#include "../Parser/parser_iface.h"
/**
 * Constructor of MainWindow class
 */
MainWindow::MainWindow()
{
    currentFile = "";

    graph = new GuiGraph();
    graph->setSceneRect( QRectF( 0, 0, 5000, 5000));
    connect( graph, SIGNAL( isClicked()), this, SLOT( textHandle()));

    view = new GuiView(graph);
    view->setScene(graph);
	//setCentralWidget( view);
    
    if( graph->getNodeItem())
      view->centerOn( graph->getNodeItem());
    
    setCurrentFile( currentFile);

    layout = new QHBoxLayout;
    layout->addWidget( view);

    widget = new QWidget;
    widget->setLayout(layout);
	setCentralWidget( widget);

    createActions();
    createMenus();
    createStatusBar();
	createDockWindows();

	gravity_timer = new QTimer ( this);
	connect( gravity_timer, SIGNAL( timeout()), this, SLOT(makeGravity()));
	gravity_timer->setInterval ( 100);
}

/**
 * Load a file
 */
void MainWindow::load()
{
    currentFile = QFileDialog::getOpenFileName( this, tr( "Open File"), "", tr( "XML (*.xml);;All files(*.*)")); 
    if ( currentFile.isEmpty()) return;
    QByteArray curFile = currentFile.toAscii();
    char *file;
    file = (char*) calloc( curFile.size(), sizeof(char));
    if (file==NULL) return;
    int i;
    for (i=0;i<=curFile.size();i++) file[i]=curFile[i];

    QApplication::setOverrideCursor( Qt::WaitCursor);

    if ( graph!=NULL) delete graph;
    graph = new GuiGraph( file);
    view->setScene( graph);
    connect( graph, SIGNAL( isClicked()), this, SLOT( textHandle()));

    nodeTextEdit->clear();
    nodeTextEdit->setReadOnly( true);
    saveTextButton->setEnabled( false);

    QApplication::restoreOverrideCursor();

    setCurrentFile( currentFile);
}

/**
 * Save current file
 */
void MainWindow::save()
{
    currentFile = QFileDialog::getSaveFileName( this, tr("Save File"), "", tr("XML (*.xml);;All files(*.*)")); 
    if ( currentFile.isEmpty()) return;
    QByteArray curFile = currentFile.toAscii();
    char *file;
    file = ( char*) calloc( curFile.size(), sizeof( char));
    if ( file==NULL) return;
    int i;
    for ( i=0; i<=curFile.size(); ++i) file[i] = curFile[i];
    graph->writeToXML( file);
    setCurrentFile( currentFile);
}

/**
 * Help
 */
void MainWindow::help()
{
    QMessageBox::about( this, tr("Mipt-Vis Help"),
      tr("To open a file :  On the File menu, click Open\n\n"\
         "To save the current file :  On the File menu, click Save\n\n"\
         "To zoom in and out :  Use the mouse wheel"));
}

/**
 * About
 */
void MainWindow::about()
{
    QMessageBox::about( this, tr("About Mipt-Vis"),
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
 * enableGravity
 */
void MainWindow::enableGravity()
{
	gravity_timer->start();
}

/**
 * disableGravity
 */
void MainWindow::disableGravity()
{
	gravity_timer->stop();
}

/**
 * enableGravity
 */
void MainWindow::makeGravity()
{
	graph->iterateGravity();
}

void MainWindow::switchVnodesShow()
{
	VirtualNodesDrawing = !VirtualNodesDrawing;
}

/**
 * convertDumpToXML
 */
void MainWindow::convertDumpToXMLSlot()
{
    currentFile = QFileDialog::getOpenFileName( this, tr( "Select file to convert"), "", tr( "All files(*.*)")); 
    if ( currentFile.isEmpty()) return;
    QByteArray curFile = currentFile.toAscii();
    char *file;
    file = (char*) calloc( curFile.size(), sizeof(char));
    if (file==NULL) return;
    int i;
    for (i=0;i<=curFile.size();i++) file[i]=curFile[i];

	convertDumpToXML( file);
} 


/**
 * centreOnNode
 */
void MainWindow::centreOnNode()
{
    //int nodeId = QInputDialog::getInteger( this, "Center On Node", "Enter Node Number", 0, -1000, 1000, 1, 0, 0);
	int nodeId = spinBox->value();
    doCentreOnNode( nodeId);
} 

/**
 * doCentreOnNode
 */
void MainWindow::doCentreOnNode( int nodeNumber)
{
    GuiNode * node;
    for ( node = ( GuiNode *)graph->firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if ( node->userId() == nodeNumber)
      {
		  view->centerOn( node);
          break;
	  }
} 

/**
 * createDockWindows
 */
void MainWindow::createDockWindows()
{
	/* Dock 1*/
    QDockWidget *dock = new QDockWidget( tr("Node description"), this);
    dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    nodeTextEdit = new GuiTextEdit;
    nodeTextEdit->clear();
    nodeTextEdit->setReadOnly( true);
	connect(nodeTextEdit, SIGNAL( nodeToBeCentreOn( int)), this, SLOT(doCentreOnNode( int)));
    
    saveTextButton = new QPushButton( tr( "Save &Text"));
    saveTextButton->setEnabled( false);
    saveTextButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    QObject::connect( saveTextButton, SIGNAL( clicked()), this, SLOT( saveTextToNode()));

    textLayout = new QVBoxLayout( dock);
    textLayout->addWidget( nodeTextEdit);
    textLayout->addWidget( saveTextButton);

    widget1 = new QWidget;
    widget1->setLayout( textLayout);

	dock->setWidget( widget1);
    addDockWidget( Qt::RightDockWidgetArea, dock);
	viewMenu->addAction(dock->toggleViewAction());
	/* Dock 2*/
	
    dock = new QDockWidget( tr("Centre on node"), this);
    dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	spinBox = new QSpinBox;
	spinBox->setRange(0,100);

    centreOnNodeButton = new QPushButton( tr("Jump"));
    centreOnNodeButton->setEnabled( true);
    centreOnNodeButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    QObject::connect( centreOnNodeButton, SIGNAL( clicked()), this, SLOT( centreOnNode()));

	centreOnNodeLayout = new QVBoxLayout( dock);
	centreOnNodeLayout->addWidget( spinBox);
	centreOnNodeLayout->addWidget( centreOnNodeButton);

    widget2 = new QWidget;
    widget2->setLayout( centreOnNodeLayout);

	dock->setWidget(widget2);
    addDockWidget( Qt::RightDockWidgetArea, dock);
	viewMenu->addAction(dock->toggleViewAction());
}

/**
 * Creat actions
 */
void MainWindow::createActions()
{
    loadAct = new QAction( tr( "&Load"), this);
    loadAct->setStatusTip( tr( "Load..."));
    connect( loadAct, SIGNAL( triggered()), this, SLOT( load()));

    saveAct = new QAction( tr( "&Save"), this);
    saveAct->setStatusTip( tr( "Save..."));
    connect( saveAct, SIGNAL( triggered()), this, SLOT( save()));

    helpAct = new QAction( tr( "Mipt-Vis &Help"), this);
    helpAct->setStatusTip( tr( "Help..."));
    connect( helpAct, SIGNAL( triggered()), this, SLOT( help()));

    aboutAct = new QAction( tr( "&About Mipt-Vis"), this);
    aboutAct->setStatusTip( tr( "About..."));
    connect( aboutAct, SIGNAL(triggered()), this, SLOT( about()));

    doLayoutAct = new QAction( tr( "Do &Layout"), this);
    doLayoutAct->setStatusTip( tr( "Do Layout..."));
    connect( doLayoutAct, SIGNAL( triggered()), this, SLOT( doLayoutSlot()));

    convertDumpToXMLAct = new QAction( tr( "&Convert dump to XML..."), this);
    convertDumpToXMLAct->setStatusTip( tr( "Convert dump to XML..."));
    connect( convertDumpToXMLAct, SIGNAL( triggered()), this, SLOT( convertDumpToXMLSlot()));
	
	enGravityAct = new QAction ( tr( "&Enable Gravity Correction"), this);
    enGravityAct->setStatusTip ( tr( "&Enable Gravity Correction"));
	disGravityAct = new QAction ( tr( "&Disable Gravity Correction"), this);
    disGravityAct->setStatusTip ( tr( "&Disable Gravity Correction"));
    connect( enGravityAct, SIGNAL( triggered()), this, SLOT( enableGravity()));
	connect( disGravityAct, SIGNAL( triggered()), this, SLOT( disableGravity()));

	
    convertDumpToXMLAct = new QAction( tr( "&Show virtual nodes"), this);
    convertDumpToXMLAct->setStatusTip( tr( "Show virtual nodes"));
    connect( convertDumpToXMLAct, SIGNAL( triggered()), this, SLOT( switchVnodesShow()));
}

/**
 * Creat menus
 */
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File"));
    fileMenu->addAction( loadAct);
    fileMenu->addAction( saveAct);

    viewMenu = menuBar()->addMenu( tr( "&View"));

    toolsMenu = menuBar()->addMenu( tr( "&Tools"));
    toolsMenu->addAction( doLayoutAct);
    toolsMenu->addAction( enGravityAct);
    toolsMenu->addAction( disGravityAct);
    toolsMenu->addAction( convertDumpToXMLAct);

    helpMenu = menuBar()->addMenu( tr( "&Help"));
    helpMenu->addAction( helpAct);
    helpMenu->addAction( aboutAct);
}

/**
 * Creat status bar
 */
void MainWindow::createStatusBar()
{
    statusBar()->showMessage( tr( "Ready"));
}

/**
 * Text Handle
 */
void MainWindow::textHandle()
{
    QList<QGraphicsItem*> list = graph->selectedItems();
    if ( list.size() == 1)
    {
		if ( qgraphicsitem_cast< GuiNode*>( list[ 0]))
		{
			GuiNode *node = qgraphicsitem_cast< GuiNode*>( list[ 0]);
			nodeTextEdit->setPlainText( node->myText);
			nodeTextEdit->setReadOnly( false);
			saveTextButton->setEnabled( true);
		}
    }
    else
    {
        nodeTextEdit->clear();
        nodeTextEdit->setReadOnly( true);
        saveTextButton->setEnabled( false);
    }
}

/**
 * saveTextToNode
 */
void MainWindow::saveTextToNode()
{
    QList<QGraphicsItem*> list = graph->selectedItems();
    if ( list.size() == 1)
    {
		GuiNode *node = qgraphicsitem_cast< GuiNode*>( list[0]);
        node->setMyText(nodeTextEdit->toPlainText());
		node->textChange();
        nodeTextEdit->clear();
        nodeTextEdit->setReadOnly( true);
        saveTextButton->setEnabled( false);
    }
}

/**
 * setCurrentFile
 */
void MainWindow::setCurrentFile( const QString & fileName)
{
    QString shownName;
    if ( fileName.isEmpty()) shownName = "untitled.xml";
    else shownName = strippedName( fileName);
    setWindowTitle( tr("%1[*] - %2").arg( shownName).arg( tr("MIPT-Vis")));
}

/**
 * strippedName
 */
QString MainWindow::strippedName( const QString &fullFileName)
{
    return QFileInfo( fullFileName).fileName();
} 
