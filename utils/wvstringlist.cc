/*
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Some helper functions for WvStringList.
 * 
 * This is blatantly block-copied from WvStringTable, but I don't care!  Hah!
 * (I just know I'm going to regret this someday...)
 */
#include "wvstringlist.h"
#include "strutils.h"


WvString WvStringList::join(const char *joinchars) const
{
    return ::strcoll_join(*this, joinchars);
}

void WvStringList::split(WvStringParm s, const char *splitchars,
    int limit)
{
    return ::strcoll_split(*this, s, splitchars, limit);
}

void WvStringList::splitstrict(WvStringParm s, const char *splitchars,
    int limit)
{
    return ::strcoll_splitstrict(*this, s, splitchars, limit);
}

void WvStringList::fill(const char * const *array)
{
    while (array && *array)
    {
	append(new WvString(*array), true);
	array++;
    }
}

WvString WvStringList::popstr()
{
    if (isempty())
	return "";
    
    WvString s = *first();
    unlink_first();
    return s;
}
