/**
 * File: GUI/GuiMW.h - Implementational header for MainWindow of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "gui_impl.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class GuiView;
class QHBoxLayout;
class QVBoxLayout;
class QWidget;
class QSplitter;
class QString;
class GuiGraph;
class GuiTextEdit;
class QGroupBox;
class QPushButton;
QT_END_NAMESPACE

/**
 * Class MainWindow
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void load();
    void save();
    void help();
    void about();
    void doLayoutSlot();
    void textHandle();
    void centreOnNode();
    void convertDumpToXmlSlot();
    void doCentreOnNode( int nodeNumber);
    void saveTextToNode();

	void enableGravity();
	void disableGravity();
	void makeGravity();
	void switchVnodesShow();

private:
    void createMenus();
    void createActions();
    void createStatusBar();
    void setCurrentFile( const QString & fileName);
	void createDockWindows();
	void createToolBars();

    QString strippedName( const QString &fullFileName);
    
    QMenu *file_menu;
    QMenu *view_menu;
    QMenu *tools_menu;
    QMenu *help_menu;

    QAction *load_act;
    QAction *save_act;
    QAction *help_act;
    QAction *about_act;
    QAction *do_layout_act;
    QAction *convert_dump_to_xml_act;
    QAction *en_gravity_act;
    QAction *dis_gravity_act;
    QAction *show_virtual_nodes_act;

	QToolBar *tool_bar;

	QTimer *gravity_timer;

    GuiView *view;
    QHBoxLayout *view_layout;
    QWidget *view_widget;
    QWidget *node_text_widget;
    QWidget *centre_on_node_widget;
    GuiGraph *graph;
    QString current_file;
    GuiTextEdit *node_text_edit;
    QVBoxLayout *node_text_layout;
    QVBoxLayout *centre_on_node_layout;
    QPushButton *save_text_button;
    QPushButton *centre_on_node_button;
	QSpinBox *centre_on_node_spin_box;

};

#endif
