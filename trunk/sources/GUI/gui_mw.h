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
    void doCentreOnNode(int nodeNumber);
    void saveTextToNode();

	void enableGravity();
	void disableGravity();
	void makeGravity();

private:
    void createMenus();
    void createActions();
    void createStatusBar();
    void setCurrentFile(const QString & fileName);
    QString strippedName(const QString &fullFileName);
    
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *loadAct;
    QAction *saveAct;
    QAction *helpAct;
    QAction *aboutAct;
    QAction *doLayoutAct;
    QAction *centreOnNodeAct;
    QAction *enGravityAct;
    QAction *disGravityAct;

	QTimer *gravity_timer;

    GuiView *view;
    QHBoxLayout *layout;
    QWidget *widget;
    QSplitter *splitter;
    GuiGraph *graph;
    QString currentFile;
    GuiTextEdit *nodeTextEdit;
    QGroupBox *groupBox;
    QVBoxLayout *textLayout;
    QVBoxLayout *rightLayout;
    QPushButton *confirmButton;
};

#endif
