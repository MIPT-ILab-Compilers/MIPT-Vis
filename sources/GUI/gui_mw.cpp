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
    current_file = "";
	setCurrentFile( current_file);

    graph = new GuiGraph();
    graph->setSceneRect( QRectF( 0, 0, 5000, 5000));
    connect( graph, SIGNAL( isClicked()), this, SLOT( textHandle()));

    view = new GuiView(graph);
    view->setScene(graph);
	
    if( graph->getNodeItem())
      view->centerOn( graph->getNodeItem());
       

    view_layout = new QHBoxLayout;
    view_layout->addWidget( view);

    view_widget = new QWidget;
    view_widget->setLayout(view_layout);
	setCentralWidget( view_widget);

    createActions();
    createMenus();
	createToolBars();
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
    current_file = QFileDialog::getOpenFileName( this, tr( "Open File"), "", tr( "XML (*.xml);;All files(*.*)")); 
    if ( current_file.isEmpty()) return;
    QByteArray cur_file = current_file.toAscii();
    char *file;
    file = (char*) calloc( cur_file.size(), sizeof(char));
    if (file==NULL) return;
    int i;
    for (i=0;i<=cur_file.size();i++) file[i]=cur_file[i];

    QApplication::setOverrideCursor( Qt::WaitCursor);

    if ( graph!=NULL) delete graph;
    graph = new GuiGraph( file);
    view->setScene( graph);
    connect( graph, SIGNAL( isClicked()), this, SLOT( textHandle()));

    node_text_edit->clear();
    node_text_edit->setReadOnly( true);
    save_text_button->setEnabled( false);

    QApplication::restoreOverrideCursor();

    setCurrentFile( current_file);
}

/**
 * Save current file
 */
void MainWindow::save()
{
    current_file = QFileDialog::getSaveFileName( this, tr("Save File"), "", tr("XML (*.xml);;All files(*.*)")); 
    if ( current_file.isEmpty()) return;
    QByteArray cur_file = current_file.toAscii();
    char *file;
    file = ( char*) calloc( cur_file.size(), sizeof( char));
    if ( file==NULL) return;
    int i;
    for ( i=0; i<=cur_file.size(); ++i) file[i] = cur_file[i];
    graph->writeToXML( file);
    setCurrentFile( current_file);
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

/*
 *Switch showind virtual nodes in graph
 */
void MainWindow::switchVnodesShow()
{
	graph->switchVnodesShow();
}
/**
 * convertDumpToXML
 */
void MainWindow::convertDumpToXMLSlot()
{
	convertDumpToXML( this);
} 


/**
 * centreOnNode
 */
void MainWindow::centreOnNode()
{
	int nodeId = centre_on_node_spin_box->value();
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
	/* centreOnNode dock*/
	QDockWidget *dock = new QDockWidget( tr("Centre on node"), this);
    dock->setAllowedAreas( Qt::AllDockWidgetAreas);
	dock->setFloating( false);

	centre_on_node_spin_box = new QSpinBox;
	centre_on_node_spin_box->setRange(0,100);

    centre_on_node_button = new QPushButton( tr("Jump"));
    centre_on_node_button->setEnabled( true);
    centre_on_node_button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    QObject::connect( centre_on_node_button, SIGNAL( clicked()), this, SLOT( centreOnNode()));

	centre_on_node_layout = new QVBoxLayout( dock);
	centre_on_node_layout->addWidget( centre_on_node_spin_box);
	centre_on_node_layout->addWidget( centre_on_node_button);

    centre_on_node_widget = new QWidget;
    centre_on_node_widget->setLayout( centre_on_node_layout);

	dock->setWidget(centre_on_node_widget);
    addDockWidget( Qt::RightDockWidgetArea, dock);
	view_menu->addAction(dock->toggleViewAction());

	/* nodeTextEdit dock*/
    dock = new QDockWidget( tr("Node description"), this);
    dock->setAllowedAreas( Qt::AllDockWidgetAreas);
	dock->setFloating( false);

    node_text_edit = new GuiTextEdit;
    node_text_edit->clear();
    node_text_edit->setReadOnly( true);
	connect(node_text_edit, SIGNAL( nodeToBeCentreOn( int)), this, SLOT(doCentreOnNode( int)));
    
    save_text_button = new QPushButton( tr( "Save &Text"));
    save_text_button->setEnabled( false);
    save_text_button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    QObject::connect( save_text_button, SIGNAL( clicked()), this, SLOT( saveTextToNode()));

    node_text_layout = new QVBoxLayout( dock);
    node_text_layout->addWidget( node_text_edit);
    node_text_layout->addWidget( save_text_button);

    node_text_widget = new QWidget;
    node_text_widget->setLayout( node_text_layout);

	dock->setWidget( node_text_widget);
    addDockWidget( Qt::RightDockWidgetArea, dock);
	view_menu->addAction(dock->toggleViewAction());
}

/**
 * Creat actions
 */
void MainWindow::createActions()
{
    load_act =  new QAction( QIcon("../GUI/images/load.bmp"),tr("&Load"), this);
    load_act->setStatusTip( tr( "Load..."));
    connect( load_act, SIGNAL( triggered()), this, SLOT( load()));

    save_act = new QAction( QIcon("../GUI/images/save.bmp"),tr( "&Save"), this);
    save_act->setShortcuts(QKeySequence::Save);
    save_act->setStatusTip( tr( "Save..."));
    connect( save_act, SIGNAL( triggered()), this, SLOT( save()));

    help_act = new QAction( tr( "Mipt-Vis &Help"), this);
    help_act->setStatusTip( tr( "Help..."));
    connect( help_act, SIGNAL( triggered()), this, SLOT( help()));

    about_act = new QAction( tr( "&About Mipt-Vis"), this);
    about_act->setStatusTip( tr( "About..."));
    connect( about_act, SIGNAL(triggered()), this, SLOT( about()));

    do_layout_act = new QAction( QIcon("../GUI/images/setLayout.bmp"),tr( "Do &Layout"), this);
    do_layout_act->setStatusTip( tr( "Do Layout..."));
    connect( do_layout_act, SIGNAL( triggered()), this, SLOT( doLayoutSlot()));

    convert_dump_to_XML_act = new QAction( tr( "&Convert dump to XML..."), this);
    convert_dump_to_XML_act->setStatusTip( tr( "Convert dump to XML..."));
    connect( convert_dump_to_XML_act, SIGNAL( triggered()), this, SLOT( convertDumpToXMLSlot()));
	
	en_gravity_act = new QAction ( QIcon("../GUI/images/enGravityAct.bmp"),tr( "&Enable Gravity Correction"), this);
    en_gravity_act->setStatusTip ( tr( "Enable Gravity Correction"));
	dis_gravity_act = new QAction (QIcon("../GUI/images/disGravityAct.bmp"),tr( "&Disable Gravity Correction"), this);
    dis_gravity_act->setStatusTip ( tr( "Disable Gravity Correction"));
    connect( en_gravity_act, SIGNAL( triggered()), this, SLOT( enableGravity()));
	connect( dis_gravity_act, SIGNAL( triggered()), this, SLOT( disableGravity()));

	
    show_virtual_nodes_act = new QAction( tr( "Show &pseudonodes trigger"), this);
    show_virtual_nodes_act->setStatusTip( tr( "Show pseudonodes trigger"));
    connect( show_virtual_nodes_act, SIGNAL( triggered()), this, SLOT( switchVnodesShow()));
}

/**
 * Creat menus
 */
void MainWindow::createMenus()
{
    file_menu = menuBar()->addMenu( tr( "&File"));
    file_menu->addAction( load_act);
    file_menu->addAction( save_act);

    view_menu = menuBar()->addMenu( tr( "&View"));

    tools_menu = menuBar()->addMenu( tr( "&Tools"));
    tools_menu->addAction( do_layout_act);
    tools_menu->addAction( en_gravity_act);
    tools_menu->addAction( dis_gravity_act);
    tools_menu->addAction( convert_dump_to_XML_act);
    tools_menu->addAction( show_virtual_nodes_act);

    help_menu = menuBar()->addMenu( tr( "&Help"));
    help_menu->addAction( help_act);
    help_menu->addAction( about_act);
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
			node_text_edit->setPlainText( node->gui_node_text);
			node_text_edit->setReadOnly( false);
			save_text_button->setEnabled( true);
		}
    }
    else
    {
        node_text_edit->clear();
        node_text_edit->setReadOnly( true);
        save_text_button->setEnabled( false);
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
        node->setGuiNodeText(node_text_edit->toPlainText());
		node->textChange();
        node_text_edit->clear();
        node_text_edit->setReadOnly( true);
        save_text_button->setEnabled( false);
    }
}

/**
 * setCurrentFile
 */
void MainWindow::setCurrentFile( const QString & file_name)
{
    QString shown_name;
    if ( file_name.isEmpty()) shown_name = "untitled.xml";
    else shown_name = strippedName( file_name);
    setWindowTitle( tr("%1[*] - %2").arg( shown_name).arg( tr("MIPT-Vis")));
}

/**
 * strippedName
 */
QString MainWindow::strippedName( const QString &full_file_name)
{
    return QFileInfo( full_file_name).fileName();
} 
/**
 * createToolBars
 */
void MainWindow::createToolBars()
{
    tool_bar = addToolBar(tr("File"));
	tool_bar->addAction(load_act);
    tool_bar->addAction(save_act);
	tool_bar->addAction(do_layout_act);
	tool_bar->addAction(en_gravity_act);
	tool_bar->addAction(dis_gravity_act);
}