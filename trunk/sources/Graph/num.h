/**
 * File: num.h - Interface and implementation of numeration functionality.
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 *
 *
 * Numerations can be used to assign numbers to objects.
 * Usage model is following. To make an object numerable you derive it from 'Numbered' class.
 * Then if you have some class representing a container for this objects you derive it from
 * the 'NumManager' class. Altarnatively you can create an instance of NumManager directly.
 * When you want to numerate objects derived from 'Numbered' you ask NumManager ( or derived)
 * for new numeration using NumManager::NewNum(). Then use Numbered::SetNumber and 
 * Numbered::GetNumber to number objects and retrieve object number.
 * When you done with using numbers free the numeration by calling NumManager::FreeNum().
 * You can have not more than MAX_NUMERATIONS ( usually 10) at one time so if you forget to 
 * free a numeration you'll run into exception telling you that there are no free numeration
 * indexes left.
 */
typedef unsigned short int NumIndex;
typedef unsigned int NumValue;

/**
 * Possible num errors
 */
typedef enum NumErrorType_e
{
    /** Some error occured */
    NUM_ERROR_GENERIC,
    /** We've ran out of indexes. Someone forgot to free nums? */
    NUM_ERROR_OUT_OF_INDEXES,
    /** We're out of values. Seems to be interanl error of num class.*/
    NUM_ERROR_OUT_OF_VALUES,
    /** Number is too big */
    NUM_ERROR_NUMBER_OUT_OF_RANGE,
    /** Number of error types */
    NUM_ERROR_NUM
} NumErrorType;

/**
 * Num description
 */
class Numeration
{
    /** Nums index */
    NumIndex index;
    /** Value */
    NumValue value;

    /** Two classes have acces to num internals. All others do not. */
    friend class Numbered;
    friend class NumManager;
};

/**
 * Num-related constants
 */
const short int MAX_NUMERATIONS = 10; /** How many numerations are allowed simultaneously */
const NumValue NUM_VAL_CLEAN = 0;
const NumValue NUM_VAL_FIRST = 1;
const NumValue NUM_VAL_LAST = ( NumValue)( (int)-1);
const GraphNum NUMBER_NO_NUM = ( GraphNum) -1;
const GraphNum NUMBER_MAX = (( GraphNum) -1) - 1;

/**
 * Represents an object that can be involved in numeration
 */
class Numbered
{
    /** Numeration descriptions */
    NumValue nums[ MAX_NUMERATIONS];

    /** Number in each numeration */
    GraphNum numbers[ MAX_NUMERATIONS];
public:
    
    Numbered()
    {
        NumIndex i;

        /** Initialize nums */
        for ( i = 0; i < MAX_NUMERATIONS; i++)
        {
            nums [ i] = NUM_VAL_CLEAN;
        }
    }

    /**
     * Mark node with num. Return false if node is already numbered. True otherwise.
     */
    inline bool SetNumber( Numeration num, GraphNum number)
    {
        if ( number >= NUMBER_MAX)
            throw NUM_ERROR_NUMBER_OUT_OF_RANGE;

        nums[ num.index] = num.value;
        numbers[ num.index] = number;
        if ( nums[ num.index] == num.value)
        {
            return false;
        }
        return true;
    }
    
    /**
     * Return number in given numeration or NO_NUM if it was not numbered yet
     */
    inline GraphNum GetNumber( Numeration num)
    {
        if ( nums[ num.index] == num.value)
        {
            return numbers[ num.index];
        }
        return NUMBER_NO_NUM;
    }

    /**
     * Return true if node is numbered in this numeration
     */
    inline bool IsNumbered( Numeration num)
    {
        if ( nums[ num.index] == num.value)
        {
            return true;
        }
        return false;
    }

    /**
     * Return true if node has been numbered in this numeration and unmarks it
     */
    inline bool UnNumber( Numeration num)
    {
        if ( nums[ num.index] == num.value)
        {
            nums[ num.index] = NUM_VAL_CLEAN;
            return true;
        }
        return false;
    }
};


/**
 * Class that creates/frees nums
 */
class NumManager
{
    /**
     * Nums
     */
    NumValue nums[ MAX_NUMERATIONS];
    bool is_used[ MAX_NUMERATIONS];
    NumValue last;

    /** Implementatinal routines */
    /**
     * Find free index
     */
    inline NumIndex FindFreeIndex()
    {
        NumIndex i = 0;
        /** Search for free num index */
        for ( i = 0; i < MAX_NUMERATIONS; i++)
        {
            if ( !is_used [ i])
            {
                return i;
            }
        }
        throw NUM_ERROR_OUT_OF_INDEXES;
        return i;
    }
    
    /**
     * Increment num value
     */
    inline NumValue GetNextValue()
    {
        if ( last == NUM_VAL_LAST)
        {
            last = NUM_VAL_FIRST;
        } else
        {
            last++;
        }
        return last;
    }
 
    /**
     * Check if this value is busy
     */
    inline bool IsValueBusy( NumValue val)
    {
        /** Check all nums */
        for ( NumIndex i = 0; i < MAX_NUMERATIONS; i++)
        {
            if ( is_used [ i] && nums[ i] == val)
                return true;
        }
        return false;
    }

    /**
     * Return next free value
     */
    inline NumValue FindNextFreeValue()
    {
        NumIndex i = 0;
        bool reached_limit = false;
        NumValue res = last;
        
        while( IsValueBusy( res))
        {
            /** 
             * If we reached checked NUM_VAL_LAST twice,
             * then we are in infinite loop because for 
             * some reason we don't free our numerations
             */
            if ( res == NUM_VAL_LAST)
            {
                Assert< NumErrorType> ( !reached_limit, 
                                           NUM_ERROR_OUT_OF_VALUES);
                reached_limit = true;            
            }
            res = GetNextValue();
        }
        return res;
    }

public:

    /**
     * Default Constructor
     */
    NumManager()
    {
        NumIndex i;

        /** Initialize nums */
        for ( i = 0; i < MAX_NUMERATIONS; i++)
        {
            nums [ i] = NUM_VAL_CLEAN;
            is_used [ i] = false;
        }
        last = NUM_VAL_FIRST;
    }
    
    /**
     * Acquire new num. Nums MUST be freed after use,
     * otherwise you run to numerations number limit.
     */
    Numeration NewNum()
    {
        Numeration new_num;
            
        new_num.index = FindFreeIndex();
        is_used[ new_num.index] = true;
        new_num.value = FindNextFreeValue();
        nums[ new_num.index] = new_num.value;
        return new_num;
    }

    /**
     * Free num
     */
    inline void FreeNum( Numeration n)
    {
        is_used[ n.index] = false;
    }
};

