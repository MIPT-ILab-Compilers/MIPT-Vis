/**
 * File: GUI/gui_iface.h - Interface of GUI library,
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Lev Rumyantsev, Ivan Mashintsev
 */
#ifndef GUI_IFACE_H
#define GUI_IFACE_H

#include "predecls.h"

bool showGraph( int argc, char * argv[], Graph * graph);

bool showScene( int argc, char * argv[], Graph * graph);

#endif