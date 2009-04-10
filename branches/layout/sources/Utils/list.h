/**
 * File: list.h - list implementation.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef LIST_H
#define LIST_H

/**
 * Types of direction in lists.
 * Lists are built from left to right by default.
 * That means if you take next in default direction - it will be element to the right.
 */
enum ListDir
{
    LIST_DIR_RIGHT = 0,
    /** Defult direction */
    LIST_DIR_DEFAULT = LIST_DIR_RIGHT,
    LIST_DIR_LEFT = 1,
    LIST_DIR_RDEFAULT = LIST_DIR_LEFT,
    /** Number of directions */
    LIST_DIR_NUM = 2
};

/**
 * Return direction that is reverse to given one
 */
inline ListDir
listRDir( ListDir dir)
{
    assert( LIST_DIR_NUM == 2);
    return ( dir == LIST_DIR_DEFAULT)? LIST_DIR_RDEFAULT: LIST_DIR_DEFAULT; 
}


/**
 * Class for objects that should have pointers to next/prev objects of their type( i.e. behave like list elements)
 * Implements headless list data structure
 */
template <class Data> class ListItem
{
    ListItem<Data> * peer[ LIST_DIR_NUM];
    Data *data_p;
public:
    
    /**get data */
    inline Data *data() const
    {
        return data_p;
    }
    /** set data */
    inline void setData( Data* d)
    {
        data_p = d;
    }
    
    /** get neighbour */
    inline ListItem<Data> * peerInDir( ListDir dir) const
    {
        return peer[ dir];
    }
    
    /** set neighbour */
    inline void setPeerInDir( ListItem<Data> *p, ListDir dir)
    {
        peer[ dir] = p;
    }
    /** Default peers gets */
    /** Return next peer in default direction */
    inline ListItem<Data> *next() const
    {
        return peerInDir( LIST_DIR_DEFAULT);
    }
    /** Return prev peer in default direction */
    inline ListItem<Data>* prev() const
    {
        return peerInDir( LIST_DIR_RDEFAULT);
    }
    /** Default peer sets */
    inline void setNext( ListItem<Data> *n)
    {
        setPeerInDir( n, LIST_DIR_DEFAULT);
    }
    inline void setPrev( ListItem<Data> *p)
    {
        setPeerInDir( p, LIST_DIR_RDEFAULT);
    }
    
    /** Attach this item to peeer in given direction */
    inline void AttachInDir( ListItem<Data>* p, ListDir dir)
    {
        ListDir rdir = listRDir( dir);
        setPeerInDir( p, dir);
        setPeerInDir( NULL, rdir);

        if ( isNotNullP( p))
        {
            ListItem<Data>* rdir_peer = p->peerInDir( rdir);
            if ( isNotNullP( rdir_peer))
            {
                rdir_peer->setPeerInDir( this, dir);
            }
            p->setPeerInDir( this, rdir);
            setPeerInDir( rdir_peer, rdir);
        }
    }
    
    /** Attach in default direction */
    inline void Attach( ListItem<Data>* peer)
    {
        AttachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Detach from neighbours */
    inline void Detach()
    {
        /** Correct links in peers */
        if ( isNotNullP( peer[ LIST_DIR_DEFAULT]))
        {
            peer[ LIST_DIR_DEFAULT]->setPeerInDir( peer[ LIST_DIR_RDEFAULT], LIST_DIR_RDEFAULT);
        }
        if ( isNotNullP( peer[ LIST_DIR_RDEFAULT]))
        {
            peer[ LIST_DIR_RDEFAULT]->setPeerInDir( peer[ LIST_DIR_DEFAULT], LIST_DIR_DEFAULT);
        }
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    }

    /** Default constructor */
    ListItem()
    {
        setData( NULL);
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    };

    /** Constructor from data pointer */
    ListItem( Data* d)
    {
        setData( d);
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    };

    /** Insert element before the given one */
    ListItem( ListItem<Data> *peer, Data* d)
    {
        setData( d);
        AttachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Insert element in given direction */
    ListItem( ListItem<Data> *peer, ListDir dir, Data *d)
    {
        setData( d);
        AttachInDir( peer, dir);
    }

    /** Destructor */
    ~ListItem()
    {
        Detach();
    }
};

#endif