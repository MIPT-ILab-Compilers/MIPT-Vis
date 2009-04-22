/**
 * File: parser.h - Parser class definition.
 * Copyright (C) 2009  MIPTVIS
 */
#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include "../Gpaph/graph_iface.h"

/**
 * Main class of Parser tool
 */
class Parser
{
private:


public:
    Parser();
    virtual ~Parser();

    bool parseFile( char * filename);
    Graph * getGraph();
    bool writeXML();

protected:
    Graph * graph;
    virtual bool parseFromStream( istream & is) = 0; 
};


#endif //PARSER_H