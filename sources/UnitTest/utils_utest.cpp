/**
 * File: utils_utest.cpp - Implementation of testing of Utils library in MiptVis tool.
 * Copyright (C) 2009 MIPTVIS
 */

#include "utest_impl.h"
#include <time.h>

/**
 * Function that compares two list items with integer data types
 */
int CompInt( const void *x, const void *y)
{
    int res;
    if ( (*(int * const*)x) < (*(int * const*)y)) res =  -1;
    if ( (*(int * const*)x) > (*(int * const*)y)) res = 1;
    if ( (*(int * const*)x) == (*(int * const*)y)) res = 0;
    return res;

}

/**
 * Print function.
 * Print the integer list to the standart output
 */

void PrintListInt( List<int>* list)
{
    ListItem<int>* p = list->head();
    while (p != NULL)
    {
        out("%d", *(p->data()));
        p = p->next();
    }
}

bool uTestList()
{
    // Print welcome message
    out( "List test starts...");

    srand( (unsigned)time( NULL ));

    //Initialize test list
    List<int> testList;
    for (int i = 0; i<10; i++)
    {
        testList.addItem( new int( rand()%100));
    }
    
    // Print test list
    out( "Test list before sorting: ");
    PrintListInt( &testList);
    
    // Delete and add tests
    out( "Delete first element: ");
    testList.deleteItem( testList.head());
    PrintListInt( &testList);

    out( "Add an element to the end: ");
    testList.addItem( new int( rand()%100));
    PrintListInt( &testList);

    // Sort test
    testList.sort( CompInt);
    
    out( "After sorting: ");
    PrintListInt( &testList);

    return true;
}