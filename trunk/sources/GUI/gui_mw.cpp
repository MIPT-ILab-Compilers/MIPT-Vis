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
}

/**
 * Actions after menu Load choosen
 */
void MainWindow::load()
{

}

/**
 * Actions after menu Save choosen
 */
void MainWindow::save()
{

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
