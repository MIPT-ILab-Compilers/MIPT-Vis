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
class QWidget;
class QString;
class GuiGraph;
class QLabel;
class QGroupBox;
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

private:
    void createMenus();
    void createActions();

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *loadAct;
    QAction *saveAct;
    QAction *helpAct;
    QAction *aboutAct;
    GuiView *view;
    QVBoxLayout *layout;
    QWidget *widget;
    GuiGraph *graph;
    QString fileName;
    QLabel *label;
    QGroupBox *groupBox;
    QVBoxLayout *textLayout;
};

#endif
