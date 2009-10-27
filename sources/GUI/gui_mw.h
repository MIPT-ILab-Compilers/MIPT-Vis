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
class QString;
class GuiGraph;
class QPlainTextEdit;
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

private:
    void createMenus();
    void createActions();
    void createStatusBar();
    
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;
    QAction *loadAct;
    QAction *saveAct;
    QAction *helpAct;
    QAction *aboutAct;
    QAction *doLayoutAct;
    GuiView *view;
    QHBoxLayout *layout;
    QWidget *widget;
    GuiGraph *graph;
    QString fileName;
    QPlainTextEdit *nodeTextEdit;
    QGroupBox *groupBox;
    QVBoxLayout *textLayout;
    QVBoxLayout *rightLayout;
    QPushButton *confirmButton;
public slots:
    void saveTextToNode();
};

#endif
