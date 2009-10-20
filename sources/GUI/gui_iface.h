/**
 * File: GUI/gui_iface.h - Interface of GUI library,
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Lev Rumyantsev, Ivan Mashintsev
 */
#ifndef GUI_IFACE_H
#define GUI_IFACE_H

#include "predecls.h"
#include "gui_node.h"
#include "gui_point.h"
#include "gui_edge.h"
#include "gui_graph.h"
#include "gui_edge_part.h"
#include "gui_mw.h"
#include "gui_view.h"


bool showGraph( int argc, char * argv[], GraphAux * graph);

bool showScene( int argc, char * argv[], QApplication * app);

#endif