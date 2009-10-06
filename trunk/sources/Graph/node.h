/**
 * File: node.h - Node class definition, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_H
#define NODE_H

/**
 * Representation of various node attributes
 */
class NodeProperties
{
	int user_id;
    char * color_priv;
	int x_priv;
	int y_priv;
	int width_priv;
	int height_priv;
	char * label_priv;
	char * shape_priv;
public:
    /** Data reading */
    inline int userId() const
    {
        return user_id;
    }
    inline char * color() const 
    {
        return color_priv;
    }
	inline int x() const
    {
        return x_priv;
    }
	inline int y() const
    { 
        return y_priv;
    }
	inline int width() const 
    {
        return width_priv;
    }
	inline int height() const 
    { 
        return height_priv;
    }
	inline char * label() const
    { 
        return label_priv;
    }
	inline char * shape() const
    { 
        return shape_priv;
    }
    /** Data writing */ 
    inline void setUserId( int i)
    {
        user_id = i;
    }
    inline void setColor( char * color)
    {
        color_priv = color;
    }
	inline void setX( int x) 
    {
        x_priv = x;
    }
	inline void setY( int y)
    { 
        y_priv = y;
    }
	inline void setWidth( int width)
    { 
        width_priv = width;
    }
	inline void setHeight( int height)
    {
        height_priv = height;
    }
	inline void setLabel( char * label) 
    {
        label_priv = label;
    }
	inline void setShape( char * shape) 
    { 
        shape_priv = shape;
    }
};

/**
 * Node representation class. 
 */
class Node: public Marked, public Numbered, public NodeProperties
{
    /** Graph is closely connected class by implementation */
    friend class Graph;
    
    /** Connection with inclusive graph */
    int unique_id; // Unique id is given by graph and cannot be modified
    Graph * graph;// Pointer to graph
    NodeListItem my_it;//Item of graph's list

	virtual void writeByXMLWriter( xmlTextWriterPtr writer);
    
    //Lists of edges and iterators for them
    EdgeListItem *edges[ GRAPH_DIRS_NUM];
    EdgeListItem *e_it[ GRAPH_DIRS_NUM];

    inline NodeListItem* getGraphIt()
    {
        return &my_it;
    }
protected:

    /** We can't create nodes separately, do it through NewNode method of graph */
    Node( Graph *graph_p, int _id):
         unique_id( _id), graph( graph_p), my_it()
    {
        edges[ GRAPH_DIR_UP] = NULL;
        edges[ GRAPH_DIR_DOWN] = NULL;
        e_it[ GRAPH_DIR_UP] = NULL;
        e_it[ GRAPH_DIR_DOWN] = NULL;
        my_it.setData( ( Node*)this);
        setUserId( _id);
    }
    
public:

    /**
     * Destructor
     */
    ~Node();
    
    /**
     * get node's unique ID
     */
    inline int id() const
    {
        return unique_id;
    }

    /**
     * get node's corresponding graph
     */
    inline Graph * getGraph() const
    {
        return graph;
    }

    /**
     * Add edge to node in specified direction
     */
    virtual void addEdgeInDir( Edge *edge, GraphDir dir);

    /**
     * Add predecessor edge
     */
    virtual inline void addPred( Edge *edge)
    {
        addEdgeInDir( edge, GRAPH_DIR_UP);
    }

    /**
     * Add successor edge
     */
    virtual inline void addSucc( Edge *edge) 
    {
        addEdgeInDir( edge, GRAPH_DIR_DOWN);
    }

    /**
     *  Iteration through edges routines.
     *
     *  set iterator to beginning of edge list and return first edge
     */
    virtual inline Edge* firstEdgeInDir( GraphDir dir)
    {
        e_it[ dir ] = edges[ dir ];
        
        if ( e_it[ dir] == NULL)
        {
            return NULL;
        }
        return e_it[ dir ]->data();
    }
    /**
     * Advance iterator and return next edge in specified direction
     * NOTE: If end of list is reached we return NULL for first time and fail if called once again
     */
    virtual inline Edge* nextEdgeInDir( GraphDir dir)
    {
        e_it[ dir] = e_it[ dir]->next();
        return (e_it[ dir] != NULL )? e_it[ dir]->data() : NULL;
    }
    /**
     * Return true if iterator of list points to one-after-last element
     */
    inline bool endOfEdgesInDir( GraphDir dir)
    {
        return e_it [ dir] == NULL;
    }

    /** 
     * Corresponding iterators for successors/predeccessors.
     * NOTE: See firstEdgeInDir and other ...InDir routines for details
     */
    virtual inline Edge* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    virtual inline Edge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline bool endOfSuccs()
    {
        return endOfEdgesInDir( GRAPH_DIR_DOWN);
    }
    virtual inline Edge* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    virtual inline Edge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    }
    inline bool endOfPreds()
    {
        return endOfEdgesInDir( GRAPH_DIR_UP);
    }

    /**
     * Deletion of edge in specified direction
     */
    void deleteEdgeInDir( GraphDir dir, EdgeListItem* it);
    
    /**
     * delete predecessor edge
     */
    inline void deletePred( EdgeListItem* it)
    {
        deleteEdgeInDir( GRAPH_DIR_UP, it);
    }
    
    /**
     * delete successor edge
     */
    inline void deleteSucc( EdgeListItem* it)
    {
        deleteEdgeInDir( GRAPH_DIR_DOWN, it);
    }

    /**
	 * Return next node of a graph
	 */
	inline Node* nextNode()
    {
        return ( my_it.next() != NULL )? my_it.next()->data() : NULL;
    }

	/**
	 * Return previous node of a graph
	 */
    inline Node* prevNode()
    {
        return ( my_it.prev() != NULL )? my_it.prev()->data() : NULL;
    }

	/**
     * Print node in DOT format to stdout
     */
    virtual void debugPrint();
};
#endif
